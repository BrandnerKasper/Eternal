#pragma once

#include "ETpch.h"
#include "Eternal.h"
#include "Eternal/Physics/ContactListener.h"

#include "Eternal/Events/KeyEvent.h"

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

            canJump = true;
            canWallJumpR = false;
            canWallJumpL = false;
        }

        void OnDestroy()
        {
        }

        void OnUpdate(Timestep ts)
        {
            HandleSideMovement();
        }

        void OnEvent(Event& e)
        {
            EventDispatcher dispatcher(e);
            dispatcher.Dispatch<KeyPressedEvent>(ET_BIND_EVENT_FN(Player::HandleJump));
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

        bool HandleJump(KeyPressedEvent& e)
        {
            if (e.GetRepeatCount() > 0)
                return false;
            m_PlayerBody = GetComponent<PhysicsComponent>().body;

            //Handle Jump (we use impulse!)
            if (e.GetKeyCode() == ET_KEY_SPACE)
            {

                //WallJump on right Wall
                if (canWallJumpR)
                {
                    PlayJumpSound();

                    //ET_CORE_WARN("Applying Right Wall Jump Impulse: {0}, {1}", m_WallJumpRForceImpulse.x, m_WallJumpRForceImpulse.y);
                    m_PlayerBody->ApplyLinearImpulse(m_WallJumpRForceImpulse, m_PlayerBody->GetWorldCenter(), true);
                    canWallJumpR = false;
                }

                //WallJump on left Wall
                else if (canWallJumpL)
                {
                    PlayJumpSound();

                    //ET_CORE_WARN("Applying Left Wall Jump Impulse: {0}, {1}", m_WallJumpLForceImpulse.x, m_WallJumpLForceImpulse.y);
                    m_PlayerBody->ApplyLinearImpulse(m_WallJumpLForceImpulse, m_PlayerBody->GetWorldCenter(), true);
                    canWallJumpL = false;
                }

                //Normal Jump
                else if (canJump)
                {
                    PlayJumpSound();

                    //ET_CORE_WARN("Applying normal Jump Impulse: {0}, {1}", m_JumpImpulse.x, m_JumpImpulse.y);
                    m_PlayerBody->ApplyLinearImpulse(m_JumpImpulse, m_PlayerBody->GetWorldCenter(), true);

                    canJump = false;
                }
            
            }
        }

        void PlayJumpSound()
        {
            //Play Jump Sound
            auto& audioComponent = GetComponent<AudioComponent>();
            audioComponent.SetGain();
            audioComponent.SetPitch();
            audioComponent.SetLoop();
            m_Entity.GetScene()->PlayAudioFile(audioComponent.file);
        }

        void HandleJumpContact()
        {
            canJump = true;
            canWallJumpR = false; //-> Edge Case
            canWallJumpL = false; //-> Edge Case
        }

        void HandleJumpWallRContact()
        {
            canWallJumpR = true;

            canJump = false;
            canWallJumpL = false;
        }

        void HandleJumpWallLContact()
        {
            canWallJumpL = true;

            canJump = false;
            canWallJumpR = false;
        }

    private:
        b2Body* m_PlayerBody;
        float m_walkvelocity = 6.0f;
        b2Vec2 m_JumpImpulse = b2Vec2(0.0f, 8.0f);
        b2Vec2 m_WallJumpRForceImpulse = b2Vec2(-10.0f, 9.0f);
        b2Vec2 m_WallJumpLForceImpulse = b2Vec2(10.0f, 9.0f);

    private:
        b2Vec2 velocity;
        float desiredVelocity_X = 0.0f;
        float velocityChange_X = 0.0f;
        float impulse_X = 0.0f;

        bool canWallJumpR = false;
        bool canWallJumpL = false;
        bool canJump = true;
        
    };
}