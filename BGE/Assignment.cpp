#include "Assignment.h"
#include "PhysicsController.h"
#include "Sphere.h"
#include "PhysicsCamera.h"
#include "Box.h"
#include "Cylinder.h"
#include "Steerable3DController.h"
#include "Ground.h"
#include "Content.h"
#include <btBulletDynamicsCommon.h>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/euler_angles.hpp>
#include <gtx/norm.hpp>
#include "VectorDrawer.h"
#include "Utils.h"
#include "Person2.h"


using namespace BGE;

Assignment::Assignment(void)
{
	KinectUse = nullptr;


}


Assignment::~Assignment(void)
{
}


void Assignment::ResetScene()
{
	int GridWidth = 5;
	int GridHeight = 6;
	shared_ptr<PhysicsController> box[6][6] = {};

	glm::vec3 BoxScale = glm::vec3(5, 5, 5);
	glm::vec3 BoxPos = glm::vec3(0, 5, 5);

	int inity = BoxPos.y;



	for (int j = 0; j < GridWidth; j++) {
		for (int i = 0; i < GridHeight; i++) {

			box[i][j] = physicsFactory->CreateBox(BoxScale.x, BoxScale.y, BoxScale.z, BoxPos, glm::quat());
			BoxPos.y += BoxScale.y;
		}
		BoxPos.z -= BoxScale.z;
		BoxPos.y = inity;

	}



}

bool Assignment::Initialise()
{


	DragonSpawner * Dragons;


	KinectUse = make_shared<KinectDragon>(Dragons);

	Attach(KinectUse);
	KinectUse->headCamera = false;


	physicsFactory->CreateGroundPhysics();
	physicsFactory->CreateCameraPhysics();

	setGravity(glm::vec3(0, -9, 0));


	Dragons->CreateDragon(glm::vec3(-30, 15, -60));
	Dragons->CreateDragon(glm::vec3(30, 15, -80));

	

	//physicsFactory->CreateCapsuleRagdoll(glm::vec3(0, 17, 10));
	


	if (!Game::Initialise()) {
		return false;
	}



	return true;
}



void Assignment::Update()
{
	Game::Update();
}








void Assignment::Cleanup()
{
	Game::Cleanup();
}