#include "EmptyGame.h"
#include "PhysicsFactory.h"
#include "Game.h"
#include "Sphere.h"
#include "Box.h"
#include "Cylinder.h"
#include "Ground.h"
#include "Content.h"
#include "PhysicsCamera.h"
#include "Model.h"
#include "dirent.h"
#include "Utils.h"
#include "Capsule.h"




using namespace BGE;


EmptyGame::EmptyGame()
{
}


EmptyGame::~EmptyGame()
{
}

bool BGE::EmptyGame::Initialise()
{
	dynamicsWorld->setGravity(btVector3(0, -9, 0));

	physicsFactory->CreateCameraPhysics();
	physicsFactory->CreateGroundPhysics();
	return Game::Initialise();
}

void EmptyGame::Update()
{
	Game::Update();
}

