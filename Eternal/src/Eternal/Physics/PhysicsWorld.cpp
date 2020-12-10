#include "ETpch.h"
#include "PhysicsWorld.h"

namespace Eternal{

	PhysicsWorld::PhysicsWorld()
	{
		InitContactListener();
	}

	PhysicsWorld::~PhysicsWorld()
	{
	}

	b2Body* PhysicsWorld::CreateBody(const b2BodyDef* def)
	{
		return m_world->CreateBody(def);
	}

	void PhysicsWorld::DestroyBody(b2Body* body)
	{
		m_world->DestroyBody(body);
	}

	void PhysicsWorld::Update(Timestep ts)
	{
		m_world->Step(ts, velocityIterations, positionIterations);
	}

	void PhysicsWorld::InitContactListener()
	{
		m_world->SetContactListener(&m_ContactListener);
	}
}