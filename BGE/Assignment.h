#pragma once
#include "Game.h"
#include "PhysicsController.h"
#include "PhysicsFactory.h"
#include <btBulletDynamicsCommon.h>
#include "Person.h"
#include "KinectDragon.h"


namespace BGE
{

	class Assignment :
		public Game
	{
	private:



	public:
		Assignment(void);
		~Assignment(void);
		bool Initialise();
		void Update();
		void Cleanup();
		void ResetScene();
		shared_ptr<KinectDragon> KinectUse;
		shared_ptr<PhysicsController> CreateDragon(glm::vec3 pos);
	};
}