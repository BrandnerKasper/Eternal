#pragma once

#include "ETpch.h"
#include "Eternal.h"
#include "Eternal/Physics/ContactListener.h"

namespace Eternal {

    class Player : public ScriptableEntity
    {
    public:
        void OnCreate()
        {
            auto& entityname = GetComponent<TagComponent>().Tag;
            ET_INFO(entityname + " Controller created");

            m_ScriptType = ScriptType::player;

            m_PlayerBody = GetComponent<PhysicsComponent>().body;
            m_PlayerBody->GetUserData().pointer = (uintptr_t)this;
        }

        void OnDestroy()
        {
        }

        void OnUpdate(Timestep ts)
        {
            HandleMovement();
        }

        void HandleMovement()
        {
            HandleSideMovement();

            HandleJump();
        }

        void HandleSideMovement()
        {
            //Get Physics body
            m_PlayerBody = GetComponent<PhysicsComponent>().body;
            velocity = m_PlayerBody->GetLinearVelocity();

            //Handle Side Movement (we use impulse)
            desiredVelocity_X = velocity.x * 0.98f;

            if (Input::IsKeyPressed(ET_KEY_D))
            {
                desiredVelocity_X = b2Min(velocity.x + 0.1f, m_walkvelocity);
            }

            if (Input::IsKeyPressed(ET_KEY_A))
            {
                desiredVelocity_X = b2Max(velocity.x - 0.1f, -m_walkvelocity);
            }

            velocityChange_X = desiredVelocity_X - velocity.x;
            impulse_X = m_PlayerBody->GetMass() * velocityChange_X;
            m_PlayerBody->ApplyLinearImpulse(b2Vec2(impulse_X, 0.0f), m_PlayerBody->GetWorldCenter(), true);
        }

        void HandleJump()
        {
            m_PlayerBody = GetComponent<PhysicsComponent>().body;

            //Handle Jump (we use pure force)
            if (Input::IsKeyPressed(ET_KEY_SPACE))
            {
                jumpHeight = m_PlayerBody->GetLinearVelocity().y;
                if (jumpHeight == 0.0f)
                    m_PlayerBody->ApplyForceToCenter(*m_JumpForce, true);
            }
        }

    private:
        b2Body* m_PlayerBody;
        float m_walkvelocity = 5.0f;
        b2Vec2* m_JumpForce = new b2Vec2(0.0f, 1000.0f);

    private:
        b2Vec2 velocity;
        float desiredVelocity_X = 0.0f;
        float velocityChange_X = 0.0f;
        float impulse_X = 0.0f;

        float jumpHeight = 0.0f;
    };
}