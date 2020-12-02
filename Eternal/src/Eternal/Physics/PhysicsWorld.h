#pragma once
#include <box2d/box2d.h>

#include "Eternal/Core/Timestep.h"

namespace Eternal {
	
	class PhysicsWorld
	{
	public:
		PhysicsWorld();
		~PhysicsWorld();

		b2Body* CreateBody(const b2BodyDef* def);
		void Update(Timestep ts);

	private:
		//Physics
		int32 velocityIterations = 6;
		int32 positionIterations = 2;

		b2Vec2* gravity = new b2Vec2(0.0f, -10.0f);
		b2World* m_world = new b2World(*gravity);
	};
}