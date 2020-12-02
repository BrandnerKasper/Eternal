#include "ETpch.h"
#include "PhysicsWorld.h"

namespace Eternal{

	PhysicsWorld::PhysicsWorld()
	{
	}

	PhysicsWorld::~PhysicsWorld()
	{
	}

	b2Body* PhysicsWorld::CreateBody(const b2BodyDef* def)
	{
		return m_world->CreateBody(def);
	}

	void PhysicsWorld::Update(Timestep ts)
	{
		m_world->Step(ts, velocityIterations, positionIterations);
	}
}