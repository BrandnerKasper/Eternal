#pragma once

#include "ETpch.h"
#include "Eternal.h"
#include "Eternal/Physics/ContactListener.h"

namespace Eternal {

    class MoveableYPlatform : public ScriptableEntity
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
            int startPositionY = (int)GetComponent<TransformComponent>().ResetPosition.y;
            int endPositionY = startPositionY + distanceY;
            int positionY = (int)m_MoveablePlatformBody->GetPosition().y;

            if (positionY == startPositionY)
            {
                m_MoveablePlatformBody->SetLinearVelocity(m_MoveVelocity);
            }
            if (positionY == endPositionY)
            {
                m_MoveablePlatformBody->SetLinearVelocity(-m_MoveVelocity);
            }
        }

    private:
        b2Body* m_MoveablePlatformBody;
        b2Vec2 m_MoveVelocity = b2Vec2(0.0f, 3.0f);
        int distanceY = 12;
    };
}