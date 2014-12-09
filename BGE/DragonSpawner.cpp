#include "DragonSpawner.h"
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
#include "PhysicsFactory.h"
#include "Game.h"


using namespace BGE;


DragonSpawner::DragonSpawner()
{
}


DragonSpawner::~DragonSpawner()
{
}


shared_ptr<PhysicsController> DragonSpawner::CreateDragon(glm::vec3 position)
{

	glm::quat HeadUpright = glm::angleAxis(90.0f, glm::vec3(1, 0, 0));

	glm::quat q = glm::angleAxis(90.0f, glm::vec3(0, 0, 1));



	shared_ptr<PhysicsController> Body = Game::Instance()->physicsFactory->CreateBox(8, 8, 15, glm::vec3(position.x, position.y, position.z), glm::quat());
	shared_ptr<PhysicsController> Head = Game::Instance()->physicsFactory->CreateFromModel("monkey", glm::vec3(position.x, position.y + 5, position.z + 15), HeadUpright, glm::vec3(5, 5, 5));


	shared_ptr<PhysicsController> FrontLeftUpperLeg = Game::Instance()->physicsFactory->CreateCapsule(2, 1.7, glm::vec3(position.x - 7.5f, position.y - 5, position.z + 5), glm::quat());;
	shared_ptr<PhysicsController> FrontRightUpperLeg = Game::Instance()->physicsFactory->CreateCapsule(2, 1.7, glm::vec3(position.x + 7.5f, position.y - 5, position.z + 5), glm::quat());;
	shared_ptr<PhysicsController> BackLeftUpperLeg = Game::Instance()->physicsFactory->CreateCapsule(2, 1.7, glm::vec3(position.x - 7.5f, position.y - 5, position.z - 5), glm::quat());
	shared_ptr<PhysicsController> BackRightUpperLeg = Game::Instance()->physicsFactory->CreateCapsule(2, 1.7, glm::vec3(position.x + 7.5f, position.y - 5, position.z - 5), glm::quat());;


	shared_ptr<PhysicsController> FrontLeftLowerLeg = Game::Instance()->physicsFactory->CreateCapsule(2, 2, glm::vec3(position.x - 7.5f, position.y - 13, position.z + 5), glm::quat());
	shared_ptr<PhysicsController> FrontRightLowerLeg = Game::Instance()->physicsFactory->CreateCapsule(2, 2, glm::vec3(position.x + 7.5f, position.y - 13, position.z + 5), glm::quat());


	shared_ptr<PhysicsController> BackLeftLowerLeg = Game::Instance()->physicsFactory->CreateCapsule(2, 2, glm::vec3(position.x - 7.5f, position.y - 13, position.z - 5), glm::quat());
	shared_ptr<PhysicsController> BackRightLowerLeg = Game::Instance()->physicsFactory->CreateCapsule(2, 2, glm::vec3(position.x + 7.5f, position.y - 13, position.z - 5), glm::quat());


	shared_ptr<PhysicsController> UpperTail = Game::Instance()->physicsFactory->CreateCapsule(1.5, 2, glm::vec3(position.x, position.y + 5, position.z - 20), glm::quat());
	shared_ptr<PhysicsController> LowerTail = Game::Instance()->physicsFactory->CreateCapsule(1.5, 2, glm::vec3(position.x, position.y - 8, position.z - 20), glm::quat());

	shared_ptr<PhysicsController> LeftLowerWing = Game::Instance()->physicsFactory->CreateBox(0.2f, 8, 4, glm::vec3(position.x-3, position.y+9, position.z), glm::quat());
	shared_ptr<PhysicsController> RightLowerWing = Game::Instance()->physicsFactory->CreateBox(0.2f, 8, 4, glm::vec3(position.x+3, position.y+9, position.z), glm::quat());
	shared_ptr<PhysicsController> LeftUpperWing = Game::Instance()->physicsFactory->CreateBox(8, 0.2f, 4, glm::vec3(position.x-9, position.y+14, position.z), glm::quat());
	shared_ptr<PhysicsController> RightUpperWing = Game::Instance()->physicsFactory->CreateBox(8, 0.2f,4, glm::vec3(position.x+9, position.y+14, position.z), glm::quat());





	Body->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);
	Head->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);
	FrontLeftUpperLeg->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);
	FrontRightUpperLeg->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);
	BackLeftUpperLeg->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);
	BackRightUpperLeg->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);
	FrontLeftLowerLeg->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);
	FrontRightLowerLeg->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);
	BackLeftLowerLeg->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);
	BackRightLowerLeg->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);

	UpperTail->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);
	LowerTail->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);

	LeftLowerWing->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);
	RightLowerWing->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);
	LeftUpperWing->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);
	RightUpperWing->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);




	btHingeConstraint * HeadBody;
	btHingeConstraint * BodyLeftFrontLeg;
	btHingeConstraint * BodyRightFrontLeg;
	btHingeConstraint * BodyLeftBackLeg;
	btHingeConstraint * BodyRightBackLeg;
	btHingeConstraint * LeftFrontLeg;
	btHingeConstraint * RightFrontLeg;
	btHingeConstraint * LeftBackLeg;
	btHingeConstraint * RightBackLeg;
	btHingeConstraint * BodyLeftWing;
	btHingeConstraint * BodyRightWing;
	
	btFixedConstraint * LeftWing;
	btFixedConstraint * RightWing;
	;

	btConeTwistConstraint * BodyUpperTail;
	btConeTwistConstraint * UpperLowerTail;

	btTransform t1, t2;


	t1.setIdentity(); t2.setIdentity();
	t1.getBasis().setEulerZYX(0, -glm::quarter_pi<float>() * 5, 0);
	t1.setOrigin(btVector3(btScalar(0), btScalar(3), btScalar(7.5)));
	t2.getBasis().setEulerZYX(0, -glm::quarter_pi<float>() * 5, 0);
	t2.setOrigin(btVector3(btScalar(0), btScalar(-4), btScalar(-4)));

	HeadBody = new btHingeConstraint(*Body->rigidBody, *Head->rigidBody, t1, t2);

	Game::Instance()->dynamicsWorld->addConstraint(HeadBody);






	t1.setIdentity(); t2.setIdentity();
	t1.setOrigin(btVector3(btScalar(-4), btScalar(-3.5), btScalar(2)));
	t2.setOrigin(btVector3(btScalar(2.5), btScalar(2.5), btScalar(-2.5)));

	BodyLeftFrontLeg = new btHingeConstraint(*Body->rigidBody, *FrontLeftUpperLeg->rigidBody, t1, t2);
	BodyLeftFrontLeg->setLimit(btScalar(0), btScalar(0));
	Game::Instance()->dynamicsWorld->addConstraint(BodyLeftFrontLeg);


	t1.setIdentity(); t2.setIdentity();
	t1.setOrigin(btVector3(btScalar(4), btScalar(-3.5), btScalar(2)));
	t2.setOrigin(btVector3(btScalar(-2.5), btScalar(2.5), btScalar(-2.5)));

	BodyRightFrontLeg = new btHingeConstraint(*Body->rigidBody, *FrontRightUpperLeg->rigidBody, t1, t2);
	BodyRightFrontLeg->setLimit(btScalar(0), btScalar(0));
	Game::Instance()->dynamicsWorld->addConstraint(BodyRightFrontLeg);

	t1.setIdentity(); t2.setIdentity();
	t1.setOrigin(btVector3(btScalar(-4), btScalar(-3.5), btScalar(-2)));
	t2.setOrigin(btVector3(btScalar(2.5), btScalar(2.5), btScalar(2.5)));

	BodyLeftBackLeg = new btHingeConstraint(*Body->rigidBody, *BackLeftUpperLeg->rigidBody, t1, t2);
	BodyLeftBackLeg->setLimit(btScalar(0), btScalar(0));
	Game::Instance()->dynamicsWorld->addConstraint(BodyLeftBackLeg);


	t1.setIdentity(); t2.setIdentity();
	t1.setOrigin(btVector3(btScalar(4), btScalar(-3.5), btScalar(-2)));
	t2.setOrigin(btVector3(btScalar(-2.5), btScalar(2.5), btScalar(2.5)));

	BodyRightBackLeg = new btHingeConstraint(*Body->rigidBody, *BackRightUpperLeg->rigidBody, t1, t2);
	BodyRightBackLeg->setLimit(btScalar(0), btScalar(0));
	Game::Instance()->dynamicsWorld->addConstraint(BodyRightBackLeg);






	t1.setIdentity(); t2.setIdentity();
	t1.setOrigin(btVector3(btScalar(0), btScalar(-3.5), btScalar(0)));
	t2.setOrigin(btVector3(btScalar(0), btScalar(3.5), btScalar(0)));

	LeftFrontLeg = new btHingeConstraint(*FrontLeftUpperLeg->rigidBody, *FrontLeftLowerLeg->rigidBody, t1, t2);
	LeftFrontLeg->setLimit(btScalar(0), btScalar(0));
	Game::Instance()->dynamicsWorld->addConstraint(LeftFrontLeg);


	t1.setIdentity(); t2.setIdentity();
	t1.setOrigin(btVector3(btScalar(0), btScalar(-3.5), btScalar(0)));
	t2.setOrigin(btVector3(btScalar(0), btScalar(3.5), btScalar(0)));

	RightFrontLeg = new btHingeConstraint(*FrontRightUpperLeg->rigidBody, *FrontRightLowerLeg->rigidBody, t1, t2);
	RightFrontLeg->setLimit(btScalar(0), btScalar(0));

	Game::Instance()->dynamicsWorld->addConstraint(RightFrontLeg);


	t1.setIdentity(); t2.setIdentity();
	t1.setOrigin(btVector3(btScalar(0), btScalar(-3.5), btScalar(0)));
	t2.setOrigin(btVector3(btScalar(0), btScalar(3.5), btScalar(0)));

	LeftBackLeg = new btHingeConstraint(*BackLeftUpperLeg->rigidBody, *BackLeftLowerLeg->rigidBody, t1, t2);
	LeftBackLeg->setLimit(btScalar(0), btScalar(0));
	Game::Instance()->dynamicsWorld->addConstraint(LeftBackLeg);


	t1.setIdentity(); t2.setIdentity();
	t1.setOrigin(btVector3(btScalar(0), btScalar(-3.5), btScalar(0)));
	t2.setOrigin(btVector3(btScalar(0), btScalar(3.5), btScalar(0)));

	RightBackLeg = new btHingeConstraint(*BackRightUpperLeg->rigidBody, *BackRightLowerLeg->rigidBody, t1, t2);
	RightBackLeg->setLimit(btScalar(0), btScalar(0));



	Game::Instance()->dynamicsWorld->addConstraint(RightBackLeg);


	t1.setIdentity(); t2.setIdentity();
	t1.setOrigin(btVector3(btScalar(0), btScalar(0), btScalar(-8)));
	t2.setOrigin(btVector3(btScalar(0), btScalar(3.5), btScalar(0)));

	BodyUpperTail = new btConeTwistConstraint(*Body->rigidBody, *UpperTail->rigidBody, t1, t2);
	BodyUpperTail->setLimit(btScalar(glm::half_pi<float>()), btScalar(0));



	Game::Instance()->dynamicsWorld->addConstraint(BodyUpperTail);


	t1.setIdentity(); t2.setIdentity();
	t1.setOrigin(btVector3(btScalar(0), btScalar(-4), btScalar(0)));
	t2.setOrigin(btVector3(btScalar(0), btScalar(4), btScalar(0)));

	UpperLowerTail = new btConeTwistConstraint(*UpperTail->rigidBody, *LowerTail->rigidBody, t1, t2);
	UpperLowerTail->setLimit(btScalar(glm::half_pi<float>()), btScalar(0));



	Game::Instance()->dynamicsWorld->addConstraint(UpperLowerTail);


	t1.setIdentity(); t2.setIdentity();
	t1.setOrigin(btVector3(btScalar(-4), btScalar(4), btScalar(0)));
	t2.setOrigin(btVector3(btScalar(0), btScalar(-4), btScalar(0)));

	BodyLeftWing = new btHingeConstraint(*Body->rigidBody, *LeftLowerWing->rigidBody, t1, t2);
	BodyLeftWing->setLimit(btScalar(0), btScalar(0));



	Game::Instance()->dynamicsWorld->addConstraint(BodyLeftWing);


	t1.setIdentity(); t2.setIdentity();
	t1.setOrigin(btVector3(btScalar(4), btScalar(4), btScalar(0)));
	t2.setOrigin(btVector3(btScalar(0), btScalar(-4), btScalar(0)));

	BodyRightWing = new btHingeConstraint(*Body->rigidBody, *RightLowerWing->rigidBody, t1, t2);
	BodyRightWing->setLimit(btScalar(0), btScalar(0));

	Game::Instance()->dynamicsWorld->addConstraint(BodyRightWing);




	t1.setIdentity();
	t2.setIdentity();
	t1.setOrigin(btVector3(btScalar(-0.2f), btScalar(4), btScalar(0)));
	t2.setRotation(GLToBtQuat(glm::angleAxis(-30.0f, glm::vec3(1, 0, 0)))); // Not sure but this needs to be a minus angle
	t2.setOrigin(btVector3(btScalar(4), btScalar(0), btScalar(0)));
	
	
	LeftWing = new btFixedConstraint(*LeftLowerWing->rigidBody, *LeftUpperWing->rigidBody, t1, t2);
	
	Game::Instance()->dynamicsWorld->addConstraint(LeftWing);
	

	
	t1.setIdentity();
	t2.setIdentity();
	t1.setOrigin(btVector3(btScalar(0.2f), btScalar(4), btScalar(0)));;
	t2.setRotation(GLToBtQuat(glm::angleAxis(-30.0f, glm::vec3(1, 0, 0)))); // Not sure but this needs to be a minus angle
	t2.setOrigin(btVector3(btScalar(-4), btScalar(0), btScalar(0)));
	RightWing = new btFixedConstraint(*RightLowerWing->rigidBody, *RightUpperWing->rigidBody, t1, t2);
	
	Game::Instance()->dynamicsWorld->addConstraint(RightWing);

	//LeftWing->enableAngularMotor(true, 10, 10);


	return Body;
}




shared_ptr<PhysicsController> DragonSpawner::CreateDragonRagdoll(glm::vec3 position)
{

	

	glm::quat q = glm::angleAxis(90.0f, glm::vec3(1, 0, 0));


	

	shared_ptr<PhysicsController> Body = Game::Instance()->physicsFactory->CreateBox(8, 8, 15, glm::vec3(position.x, position.y, position.z), glm::quat());


	shared_ptr<PhysicsController> Head = Game::Instance()->physicsFactory->CreateFromModel("monkey", glm::vec3(position.x, position.y + 5, position.z + 15), q, glm::vec3(5, 5, 5));


	shared_ptr<PhysicsController> FrontLeftUpperLeg = Game::Instance()->physicsFactory->CreateCapsule(2, 1.7, glm::vec3(position.x - 7.5f, position.y - 5, position.z + 5), glm::quat());;
	shared_ptr<PhysicsController> FrontRightUpperLeg = Game::Instance()->physicsFactory->CreateCapsule(2, 1.7, glm::vec3(position.x + 7.5f, position.y - 5, position.z + 5), glm::quat());;
	shared_ptr<PhysicsController> BackLeftUpperLeg = Game::Instance()->physicsFactory->CreateCapsule(2, 1.7, glm::vec3(position.x - 7.5f, position.y - 5, position.z - 5), glm::quat());
	shared_ptr<PhysicsController> BackRightUpperLeg = Game::Instance()->physicsFactory->CreateCapsule(2, 1.7, glm::vec3(position.x + 7.5f, position.y - 5, position.z - 5), glm::quat());;




	shared_ptr<PhysicsController> FrontLeftLowerLeg = Game::Instance()->physicsFactory->CreateCapsule(2, 2, glm::vec3(position.x - 7.5f, position.y - 13, position.z + 5), glm::quat());
	shared_ptr<PhysicsController> FrontRightLowerLeg = Game::Instance()->physicsFactory->CreateCapsule(2, 2, glm::vec3(position.x + 7.5f, position.y - 13, position.z + 5), glm::quat());
	shared_ptr<PhysicsController> BackLeftLowerLeg = Game::Instance()->physicsFactory->CreateCapsule(2, 2, glm::vec3(position.x - 7.5f, position.y - 13, position.z - 5), glm::quat());
	shared_ptr<PhysicsController> BackRightLowerLeg = Game::Instance()->physicsFactory->CreateCapsule(2, 2, glm::vec3(position.x + 7.5f, position.y - 13, position.z - 5), glm::quat());





	shared_ptr<PhysicsController> UpperTail = Game::Instance()->physicsFactory->CreateCapsule(1.5, 2, glm::vec3(position.x, position.y + 5, position.z - 20), glm::quat());
	shared_ptr<PhysicsController> LowerTail = Game::Instance()->physicsFactory->CreateCapsule(1.5, 2, glm::vec3(position.x, position.y - 8, position.z - 20), glm::quat());

	Body->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);
	Head->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);
	FrontLeftUpperLeg->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);
	FrontRightUpperLeg->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);
	BackLeftUpperLeg->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);
	BackRightUpperLeg->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);
	FrontLeftLowerLeg->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);
	FrontRightLowerLeg->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);
	BackLeftLowerLeg->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);
	BackRightLowerLeg->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);

	UpperTail->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);
	LowerTail->transform->diffuse = glm::vec3(0.2, 0.4, 0.4);


	btHingeConstraint * HeadBody;
	btHingeConstraint * BodyLeftFrontLeg;
	btHingeConstraint * BodyRightFrontLeg;
	btHingeConstraint * BodyLeftBackLeg;
	btHingeConstraint * BodyRightBackLeg;
	btHingeConstraint * LeftFrontLeg;
	btHingeConstraint * RightFrontLeg;
	btHingeConstraint * LeftBackLeg;
	btHingeConstraint * RightBackLeg;
	btConeTwistConstraint * BodyUpperTail;
	btConeTwistConstraint * UpperLowerTail;

	btTransform t1, t2;


	t1.setIdentity(); t2.setIdentity();
	t1.getBasis().setEulerZYX(0, -glm::quarter_pi<float>() * 5, 0);
	t1.setOrigin(btVector3(btScalar(0), btScalar(3), btScalar(7.5)));
	t2.getBasis().setEulerZYX(0, -glm::quarter_pi<float>() * 5, 0);
	t2.setOrigin(btVector3(btScalar(0), btScalar(-4), btScalar(-4)));

	HeadBody = new btHingeConstraint(*Body->rigidBody, *Head->rigidBody, t1, t2);

	Game::Instance()->dynamicsWorld->addConstraint(HeadBody);






	t1.setIdentity(); t2.setIdentity();
	t1.getBasis().setEulerZYX(0, glm::half_pi<float>(), 0);
	t1.setOrigin(btVector3(btScalar(-4), btScalar(-3.5), btScalar(2)));
	t2.getBasis().setEulerZYX(0, glm::half_pi<float>(), 0);
	t2.setOrigin(btVector3(btScalar(2.5), btScalar(2.5), btScalar(-2.5)));

	BodyLeftFrontLeg = new btHingeConstraint(*Body->rigidBody, *FrontLeftUpperLeg->rigidBody, t1, t2);
	BodyLeftFrontLeg->setLimit(btScalar(glm::quarter_pi<float>()), btScalar(0));
	Game::Instance()->dynamicsWorld->addConstraint(BodyLeftFrontLeg);


	t1.setIdentity(); t2.setIdentity();
	t1.getBasis().setEulerZYX(0, glm::half_pi<float>(), 0);
	t1.setOrigin(btVector3(btScalar(4), btScalar(-3.5), btScalar(2)));
	t2.getBasis().setEulerZYX(0, glm::half_pi<float>(), 0);
	t2.setOrigin(btVector3(btScalar(-2.5), btScalar(2.5), btScalar(-2.5)));

	BodyRightFrontLeg = new btHingeConstraint(*Body->rigidBody, *FrontRightUpperLeg->rigidBody, t1, t2);
	BodyRightFrontLeg->setLimit(btScalar(glm::quarter_pi<float>()), btScalar(0));
	Game::Instance()->dynamicsWorld->addConstraint(BodyRightFrontLeg);


	t1.setIdentity(); t2.setIdentity();
	t1.getBasis().setEulerZYX(0, glm::half_pi<float>(), 0);
	t1.setOrigin(btVector3(btScalar(-4), btScalar(-3.5), btScalar(-2)));
	t2.getBasis().setEulerZYX(0, glm::half_pi<float>(), 0);
	t2.setOrigin(btVector3(btScalar(2.5), btScalar(2.5), btScalar(2.5)));

	BodyLeftBackLeg = new btHingeConstraint(*Body->rigidBody, *BackLeftUpperLeg->rigidBody, t1, t2);
	BodyLeftBackLeg->setLimit(btScalar(glm::quarter_pi<float>()), btScalar(0));
	Game::Instance()->dynamicsWorld->addConstraint(BodyLeftBackLeg);


	t1.setIdentity(); t2.setIdentity();
	t1.getBasis().setEulerZYX(0, glm::half_pi<float>(), 0);
	t1.setOrigin(btVector3(btScalar(4), btScalar(-3.5), btScalar(-2)));
	t2.getBasis().setEulerZYX(0, glm::half_pi<float>(), 0);
	t2.setOrigin(btVector3(btScalar(-2.5), btScalar(2.5), btScalar(2.5)));

	BodyRightBackLeg = new btHingeConstraint(*Body->rigidBody, *BackRightUpperLeg->rigidBody, t1, t2);
	BodyRightBackLeg->setLimit(btScalar(glm::quarter_pi<float>()), btScalar(0));
	Game::Instance()->dynamicsWorld->addConstraint(BodyRightBackLeg);






	t1.setIdentity(); t2.setIdentity();
	t1.getBasis().setEulerZYX(0, glm::half_pi<float>(), 0);
	t1.setOrigin(btVector3(btScalar(0), btScalar(-3.5), btScalar(0)));
	t2.getBasis().setEulerZYX(0, glm::half_pi<float>(), 0);
	t2.setOrigin(btVector3(btScalar(0), btScalar(3.5), btScalar(0)));

	LeftFrontLeg = new btHingeConstraint(*FrontLeftUpperLeg->rigidBody, *FrontLeftLowerLeg->rigidBody, t1, t2);
	LeftFrontLeg->setLimit(btScalar(glm::quarter_pi<float>()), btScalar(0));
	Game::Instance()->dynamicsWorld->addConstraint(LeftFrontLeg);


	t1.setIdentity(); t2.setIdentity();
	t1.getBasis().setEulerZYX(0, glm::half_pi<float>(), 0);
	t1.setOrigin(btVector3(btScalar(0), btScalar(-3.5), btScalar(0)));
	t2.getBasis().setEulerZYX(0, glm::half_pi<float>(), 0);
	t2.setOrigin(btVector3(btScalar(0), btScalar(3.5), btScalar(0)));

	RightFrontLeg = new btHingeConstraint(*FrontRightUpperLeg->rigidBody, *FrontRightLowerLeg->rigidBody, t1, t2);
	RightFrontLeg->setLimit(btScalar(glm::quarter_pi<float>()), btScalar(0));

	Game::Instance()->dynamicsWorld->addConstraint(RightFrontLeg);


	t1.setIdentity(); t2.setIdentity();
	t1.getBasis().setEulerZYX(0, glm::half_pi<float>(), 0);
	t1.setOrigin(btVector3(btScalar(0), btScalar(-3.5), btScalar(0)));
	t2.getBasis().setEulerZYX(0, glm::half_pi<float>(), 0);
	t2.setOrigin(btVector3(btScalar(0), btScalar(3.5), btScalar(0)));

	LeftBackLeg = new btHingeConstraint(*BackLeftUpperLeg->rigidBody, *BackLeftLowerLeg->rigidBody, t1, t2);
	LeftBackLeg->setLimit(btScalar(glm::quarter_pi<float>()), btScalar(0));
	Game::Instance()->dynamicsWorld->addConstraint(LeftBackLeg);


	t1.setIdentity(); t2.setIdentity();
	t1.getBasis().setEulerZYX(0, glm::half_pi<float>(), 0);
	t1.setOrigin(btVector3(btScalar(0), btScalar(-3.5), btScalar(0)));
	t2.getBasis().setEulerZYX(0, glm::half_pi<float>(), 0);
	t2.setOrigin(btVector3(btScalar(0), btScalar(3.5), btScalar(0)));

	RightBackLeg = new btHingeConstraint(*BackRightUpperLeg->rigidBody, *BackRightLowerLeg->rigidBody, t1, t2);
	RightBackLeg->setLimit(btScalar(glm::quarter_pi<float>()), btScalar(0));



	Game::Instance()->dynamicsWorld->addConstraint(RightBackLeg);


	t1.setIdentity(); t2.setIdentity();
	t1.getBasis().setEulerZYX(0, glm::half_pi<float>(), 0);
	t1.setOrigin(btVector3(btScalar(0), btScalar(0), btScalar(-8)));
	t2.getBasis().setEulerZYX(0, glm::half_pi<float>(), 0);
	t2.setOrigin(btVector3(btScalar(0), btScalar(3.5), btScalar(0)));

	BodyUpperTail = new btConeTwistConstraint(*Body->rigidBody, *UpperTail->rigidBody, t1, t2);
	BodyUpperTail->setLimit(btScalar(glm::half_pi<float>()), btScalar(0));



	Game::Instance()->dynamicsWorld->addConstraint(BodyUpperTail);


	t1.setIdentity(); t2.setIdentity();
	t1.getBasis().setEulerZYX(0, glm::half_pi<float>(), 0);
	t1.setOrigin(btVector3(btScalar(0), btScalar(-4), btScalar(0)));
	t2.getBasis().setEulerZYX(0, glm::half_pi<float>(), 0);
	t2.setOrigin(btVector3(btScalar(0), btScalar(4), btScalar(0)));

	UpperLowerTail = new btConeTwistConstraint(*UpperTail->rigidBody, *LowerTail->rigidBody, t1, t2);
	UpperLowerTail->setLimit(btScalar(glm::half_pi<float>()), btScalar(0));



	Game::Instance()->dynamicsWorld->addConstraint(UpperLowerTail);







	return Body;
}
