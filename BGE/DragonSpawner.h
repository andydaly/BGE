#pragma once
#include "Game.h"
#include "PhysicsController.h"
#include "PhysicsFactory.h"
#include <btBulletDynamicsCommon.h>
#include "Person.h"



using namespace std;

namespace BGE
{
	class DragonSpawner
	{
		private:

		public:
			DragonSpawner(void);
			~DragonSpawner(void);
			shared_ptr<Person> KinectUse;
			shared_ptr<PhysicsController> CreateDragon(glm::vec3 pos);
			shared_ptr<PhysicsController> CreateDragonRagdoll(glm::vec3 pos);
	};
}

