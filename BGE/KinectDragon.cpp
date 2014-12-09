#include "KinectDragon.h"
#include "Person.h"
#include "Game.h"
#include <sstream>
#include "Box.h"
#include "Cylinder.h"
#include "PhysicsController.h"
#include "KinematicMotionState.h"
#include "PhysicsGame1.h"
#include <btBulletDynamicsCommon.h>
#include "DragonSpawner.h"

using namespace BGE;
using namespace std;


glm::vec3 NUIToGLVector2(Vector4 v, bool flip)
{
	if (flip)
	{
		return glm::vec3(v.x, v.y, -v.z);
	}
	else
	{
		return glm::vec3(v.x, v.y, v.z);
	}
}


KinectDragon::KinectDragon(DragonSpawner *dragons):GameComponent()
{
	Dragons = dragons;
	connected = false;
	tracked = false;
	headCamera = true;
	m_pNuiSensor = NULL;
	scale = 20.0f;
	footHeight = 0.0f;

}


KinectDragon::~KinectDragon()
{
}


bool KinectDragon::Initialise()
{
	CreateFirstConnected();
	NuiSetDeviceStatusCallback(&StatusProc, this);
	return GameComponent::Initialise();
}

HRESULT BGE::KinectDragon::CreateFirstConnected()
{
	INuiSensor * pNuiSensor = NULL;

	int iSensorCount = 0;
	HRESULT hr = NuiGetSensorCount(&iSensorCount);
	if (FAILED(hr))
	{
		return hr;
	}

	// Look at each Kinect sensor
	for (int i = 0; i < iSensorCount; ++i)
	{
		// Create the sensor so we can check status, if we can't create it, move on to the next
		hr = NuiCreateSensorByIndex(i, &pNuiSensor);
		if (FAILED(hr))
		{
			continue;
		}

		// Get the status of the sensor, and if connected, then we can initialize it
		hr = pNuiSensor->NuiStatus();
		if (S_OK == hr)
		{
			m_pNuiSensor = pNuiSensor;
			break;
		}

		// This sensor wasn't OK, so release it since we're not using it
		pNuiSensor->Release();
		return hr;
	}

	if (NULL != m_pNuiSensor)
	{
		// Initialize the Kinect and specify that we'll be using skeleton
		hr = m_pNuiSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON);
		if (SUCCEEDED(hr))
		{
			// Create an event that will be signaled when skeleton data is available
			m_hNextSkeletonEvent = CreateEventW(NULL, TRUE, FALSE, NULL);
			// Open a skeleton stream to receive skeleton data
			hr = m_pNuiSensor->NuiSkeletonTrackingEnable(m_hNextSkeletonEvent, 0);
		}
	}

	if (NULL == m_pNuiSensor || FAILED(hr))
	{
		SetStatusMessage("No ready Kinect found!");
		return E_FAIL;
	}
	connected = true;
	SetStatusMessage("Kinect Connected");
	return hr;
}


void KinectDragon::UpdateSkeleton(const NUI_SKELETON_DATA & skeleton)
{
	const Uint8 * keyState = Game::Instance()->GetKeyState();
	static bool lastPressed = false;
	if (keyState[SDL_SCANCODE_C])
	{
		if (!lastPressed)
		{
			headCamera = !headCamera;
		}
		lastPressed = true;
	}
	else
	{
		lastPressed = false;
	}

	//if (footHeight == 0.0f)
	{
		footHeight = glm::min<float>(skeleton.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_RIGHT].y, skeleton.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_LEFT].y);
	}

	UpdateBone(skeleton, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_LEFT);
	UpdateBone(skeleton, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_RIGHT);
	

	UpdateBone(skeleton, NUI_SKELETON_POSITION_SHOULDER_LEFT, NUI_SKELETON_POSITION_ELBOW_LEFT);
	UpdateBone(skeleton, NUI_SKELETON_POSITION_ELBOW_LEFT, NUI_SKELETON_POSITION_WRIST_LEFT);
	UpdateBone(skeleton, NUI_SKELETON_POSITION_WRIST_LEFT, NUI_SKELETON_POSITION_HAND_LEFT);

	UpdateBone(skeleton, NUI_SKELETON_POSITION_SHOULDER_RIGHT, NUI_SKELETON_POSITION_ELBOW_RIGHT);
	UpdateBone(skeleton, NUI_SKELETON_POSITION_ELBOW_RIGHT, NUI_SKELETON_POSITION_WRIST_RIGHT);
	UpdateBone(skeleton, NUI_SKELETON_POSITION_WRIST_RIGHT, NUI_SKELETON_POSITION_HAND_RIGHT);

	//UpdateDragon(skeleton, NUI_SKELETON_POSITION_HEAD);
	UpdateDragon(skeleton, NUI_SKELETON_POSITION_HIP_CENTER);
}

void KinectDragon::UpdateKnob(string tag, glm::vec3 pos)
{
	shared_ptr<PhysicsController> knobController;
	map<string, shared_ptr<PhysicsController>>::iterator it = boneComponents.find(tag);
	if (it == boneComponents.end())
	{
		knobController = Game::Instance()->physicsFactory->CreateSphere(1.0f, pos, glm::quat(), true, true);
		knobController->rigidBody->setMotionState(new KinematicMotionState(knobController->parent));
		knobController->tag = "HandJointController";
		knobController->parent->tag = tag;
		knobController->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);
		boneComponents[tag] = knobController;
	}
	else
	{
		knobController = (*it).second;
		knobController->transform->position = pos;
	}
}


void KinectDragon::UpdateBone(
	const NUI_SKELETON_DATA & skeleton,
	NUI_SKELETON_POSITION_INDEX jointFrom,
	NUI_SKELETON_POSITION_INDEX jointTo)
{
	NUI_SKELETON_POSITION_TRACKING_STATE jointFromState = skeleton.eSkeletonPositionTrackingState[jointFrom];
	NUI_SKELETON_POSITION_TRACKING_STATE jointToState = skeleton.eSkeletonPositionTrackingState[jointTo];


	if (jointFromState == NUI_SKELETON_POSITION_NOT_TRACKED || jointToState == NUI_SKELETON_POSITION_NOT_TRACKED)
	{
		return; // nothing to draw, one of the joints is not tracked
	}

	glm::vec3 start = NUIToGLVector2(skeleton.SkeletonPositions[jointFrom], !headCamera);
	glm::vec3 end = NUIToGLVector2(skeleton.SkeletonPositions[jointTo], !headCamera);
	start.y -= footHeight;
	end.y -= footHeight;

	start *= scale;
	end *= scale;

	glm::vec3 boneVector = end - start;
	float boneLength = glm::length(boneVector);
	glm::vec3 centrePos = start + ((boneVector) / 2.0f);
	boneVector = glm::normalize(boneVector);
	glm::vec3 axis = glm::cross(Transform::basisUp, boneVector);
	axis = glm::normalize(axis);
	float theta = (float)glm::acos<float>(glm::dot<float>(Transform::basisUp, boneVector));
	glm::quat q = glm::angleAxis(glm::degrees(theta), axis);

	stringstream ss;
	ss << jointFrom << "," << jointTo;
	string boneKey = ss.str();

	map<string, shared_ptr<PhysicsController>>::iterator it = boneComponents.find(boneKey);
	shared_ptr<PhysicsController> cylController;
	if (it == boneComponents.end())
	{
		//cylController = Game::Instance()->physicsFactory->CreateCylinder(0.5f, boneLength, centrePos, transform->orientation);

		cylController = Game::Instance()->physicsFactory->CreateBox(0.2f, 10, boneLength, centrePos, transform->orientation);
		cylController->transform->diffuse = glm::vec3();

		cylController->rigidBody->setCollisionFlags(cylController->rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		cylController->rigidBody->setActivationState(DISABLE_DEACTIVATION);
		cylController->rigidBody->setMotionState(new KinematicMotionState(cylController->parent));
		cylController->tag = "PersonBone";
		boneComponents[boneKey] = cylController;
	}
	else
	{
		cylController = it->second;
	}
	cylController->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);
	cylController->transform->position = transform->position + centrePos;
	cylController->transform->orientation = q;

	UpdateKnob("" + jointFrom, start);
	UpdateKnob("" + jointTo, end);
}

void KinectDragon::UpdateDragon(
	const NUI_SKELETON_DATA & skeleton,
	NUI_SKELETON_POSITION_INDEX joint)
{
	NUI_SKELETON_POSITION_TRACKING_STATE jointFromState = skeleton.eSkeletonPositionTrackingState[joint];

	if (jointFromState == NUI_SKELETON_POSITION_NOT_TRACKED)
	{
		return; // nothing to draw, one of the joints is not tracked
	}

	glm::vec3 boneVector = NUIToGLVector2(skeleton.SkeletonPositions[joint], !headCamera);
	boneVector.y -= footHeight;

	boneVector *= scale;

	glm::quat q = glm::angleAxis(glm::degrees(glm::pi<float>()), glm::vec3(1, 0, 0));

	stringstream ss;
	ss << joint;
	string boneKey = ss.str();
	map<string, shared_ptr<PhysicsController>>::iterator it = boneComponents.find(boneKey);
	shared_ptr<PhysicsController> DragonBody;

	glm::vec3 ModelPos = glm::vec3(boneVector.x, boneVector.y, boneVector.z - 5);

	//shared_ptr<Assignment> AssignmentCreator;
	if (it == boneComponents.end())
	{
		
		DragonBody = Dragons->CreateDragonRagdoll(ModelPos);
		DragonBody->rigidBody->setCollisionFlags(DragonBody->rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		DragonBody->rigidBody->setActivationState(DISABLE_DEACTIVATION);
		DragonBody->rigidBody->setMotionState(new KinematicMotionState(DragonBody->parent));
		//DragonBody->tag = "PersonHead";

		boneComponents[boneKey] = DragonBody;
	}
	else
	{
		DragonBody = it->second;
	}

	if (headCamera)
	{
		//Game::Instance()->camera->transform->position = boneVector + glm::vec3(0, scale * 0.2f, 0);
		Game::Instance()->camera->transform->position = ModelPos + glm::vec3(0, scale * 0.2f, 0);
		DragonBody->transform->position = glm::vec3(100, -100, 100);
	}
	else
	{
		//boxController->transform->position = transform->position + boneVector;
		DragonBody->transform->position = transform->position + ModelPos;
	}
	//DragonBody->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);
	//boxController->transform->orientation = q;

}

void KinectDragon::SkeletonFrameReady(NUI_SKELETON_FRAME* pSkeletonFrame)
{
	tracked = false;
	for (int i = 0; i < NUI_SKELETON_COUNT; i++)
	{
		const NUI_SKELETON_DATA & skeleton = pSkeletonFrame->SkeletonData[i];

		switch (skeleton.eTrackingState)
		{
		case NUI_SKELETON_TRACKED:
			UpdateSkeleton(skeleton);
			tracked = true;
			// Just draw the first skeleton I find
			return;
		case NUI_SKELETON_NOT_TRACKED:
		case NUI_SKELETON_POSITION_ONLY:
			break;
		}
	}

}

void KinectDragon::Update()
{
	if (connected)
	{
		SetStatusMessage("Kinect is connected");
		SetStatusMessage("Press C to toggle the head camera");
		if (tracked)
		{
			SetStatusMessage("Kinect is tracking");
		}
		else
		{
			SetStatusMessage("Kinect is not tracking");
		}
		// Wait for 0ms, just quickly test if it is time to process a skeleton
		if (WAIT_OBJECT_0 == WaitForSingleObject(m_hNextSkeletonEvent, 0))
		{
			NUI_SKELETON_FRAME skeletonFrame = { 0 };

			// Get the skeleton frame that is ready
			if (SUCCEEDED(m_pNuiSensor->NuiSkeletonGetNextFrame(0, &skeletonFrame)))
			{
				m_pNuiSensor->NuiTransformSmooth(&skeletonFrame, &verySmoothParams);
				// Process the skeleton frame				
				SkeletonFrameReady(&skeletonFrame);
			}
		}
	}
	else
	{
		SetStatusMessage("Kinect is not connected");
	}

	GameComponent::Update();
}

void KinectDragon::SetStatusMessage(std::string message)
{
	Game::Instance()->PrintText(message);
}
