#pragma once

#include "ETpch.h"
#include "Eternal.h"
#include "Eternal/Physics/ContactListener.h"

namespace Eternal {

    class MoveableXPlatform : public ScriptableEntity
    {
    public:

        void OnCreate()
        {
            auto& entityname = GetComponent<TagComponent>().Tag;
            ET_INFO(entityname + " Moveable Platform created");
            
            m_MoveablePlatformBody = GetComponent<PhysicsComponent>().body;
            if (m_MoveablePlatformBody->GetType() != b2BodyType::b2_kinematicBody)
                ET_ERROR("Moveable Platform needs to be a kinematic Body!");
        }

        void OnDestroy()
        {
        }

        void OnUpdate(Timestep ts)
        {
            int startPositionX = (int)GetComponent<TransformComponent>().ResetPosition.x;
            int endPositionX = startPositionX + distanceX;
            int positionX = (int)m_MoveablePlatformBody->GetPosition().x;

            if (positionX == startPositionX)
            {
                m_MoveablePlatformBody->SetLinearVelocity(m_MoveVelocity);
            }
            if (positionX == endPositionX)
            {
                m_MoveablePlatformBody->SetLinearVelocity(-m_MoveVelocity);
            }
        }

    private:
        b2Body* m_MoveablePlatformBody;
        b2Vec2 m_MoveVelocity = b2Vec2(3.0f, 0.0f);
        int distanceX = 10;
    };
}