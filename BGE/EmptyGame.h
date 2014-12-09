#pragma once
#include "Game.h"

namespace BGE
{
	class EmptyGame :
		public Game
	{
	public:
		EmptyGame();
		~EmptyGame();
		shared_ptr<PhysicsController> CreateAnimat(glm::vec3 position);
		bool Initialise();
		void Update();
	};
}
