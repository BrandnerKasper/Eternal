#pragma once

#include "ETpch.h"
#include "Eternal.h"
#include "Eternal/Physics/ContactListener.h"

namespace Eternal {

    class MoveableDeathbox : public ScriptableEntity
    {
    public:

        void OnCreate()
        {
            auto& entityname = GetComponent<TagComponent>().Tag;
            ET_INFO(entityname + " Moveable Deathbox created");

            //Set ScriptType for ContactHandler
            m_ScriptType = ScriptType::deathbox;

            //Set User Data in Physics
            auto& m_MoveableDeathboxBody = GetComponent<PhysicsComponent>().body;

            if (m_MoveableDeathboxBody->GetType() != b2BodyType::b2_kinematicBody)
                ET_ERROR("Moveable Platform needs to be a kinematic Body!");

            m_MoveableDeathboxBody->GetUserData().pointer = (uintptr_t)this;

            //Set Physics Component as Sensor
            GetComponent<PhysicsComponent>().Fixture->SetSensor(true);
        }

        void OnDestroy()
        {
        }

        void OnUpdate(Timestep ts)
        {
            auto& m_MoveableDeathboxBody = GetComponent<PhysicsComponent>().body;

            int startPositionX = (int)GetComponent<TransformComponent>().ResetPosition.x;
            int endPositionX = startPositionX + distanceX;
            int positionX = (int)m_MoveableDeathboxBody->GetPosition().x;

            if (positionX == startPositionX)
            {
                m_MoveableDeathboxBody->SetLinearVelocity(m_MoveVelocity);
            }
            if (positionX == endPositionX)
            {
                m_MoveableDeathboxBody->SetLinearVelocity(-m_MoveVelocity);
            }
        }

        void HandlePlayerIsDead()
        {
            m_Entity.GetScene()->SetPlay(false);
        }

    private:
        b2Vec2 m_MoveVelocity = b2Vec2(1.0f, 0.0f);
        int distanceX = 3;
    };
}