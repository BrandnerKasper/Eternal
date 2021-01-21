#pragma once

#include "ETpch.h"
#include "Eternal.h"
#include "Eternal/Physics/ContactListener.h"

namespace Eternal {

    class SceneCameraController : public ScriptableEntity
    {
    public:

        void OnCreate()
        {
            auto& entityname = GetComponent<TagComponent>().Tag;
            ET_INFO(entityname + " Scene Camera Movement Script created");

            auto& cameraPosition = GetComponent<TransformComponent>().Position;
            auto& cameraPositionX = cameraPosition.x;
            auto& cameraPositionY = cameraPosition.y;
            
            auto& playerPosition = m_Entity.GetScene()->GetEntityByTag("Player").GetComponent<TransformComponent>().Position;
            
            auto& playerPositionX = playerPosition.x;
            auto& playerPositionY = playerPosition.y;
            
            auto newCameraPosition = glm::vec2(playerPositionX + 7.0f, playerPositionY + 3.0f);
            cameraPosition = glm::vec3(newCameraPosition, cameraPosition.z);
        }

        void OnDestroy()
        {
        }
        
        void OnUpdate(Timestep ts)
        {
            JustFollow();
        }

        void JustFollow()
        {
            auto& cameraPosition = GetComponent<TransformComponent>().Position;
            auto& cameraPositionX = cameraPosition.x;
            auto& cameraPositionY = cameraPosition.y;

            auto& playerPosition = m_Entity.GetScene()->GetEntityByTag("Player").GetComponent<TransformComponent>().Position;

            auto& playerPositionX = playerPosition.x;
            auto& playerPositionY = playerPosition.y;

            auto newCameraPosition = glm::vec2(playerPositionX + 7.0f, playerPositionY + 3.0f);

            HandleLerpX(cameraPositionX, newCameraPosition.x);
            HandleLerpY(cameraPositionY, newCameraPosition.y);
        }

        float Lerp(float a, float b, float f)
        {
            return a + f * (b - a);
        }

        void HandleLerpX(float& cameraPositionX, float& playerPositionX)
        {
            if (!m_Allowlerp_X)
            {
                if (abs(cameraPositionX - playerPositionX) > lerpDistanceX)
                    m_Allowlerp_X = true;
            }

            if (m_Allowlerp_X)
            {
                cameraPositionX = Lerp(cameraPositionX, playerPositionX, lerpSpeed);
                if (abs(cameraPositionX - playerPositionX) < lerpToleranceX)
                    m_Allowlerp_X = false;
            }
        }

        void HandleLerpY(float& cameraPositionY, float& playerPositionY)
        {
            if (!m_Allowlerp_Y)
            {
                if (playerPositionY - cameraPositionY > lerpDistanceYUp)
                    m_Allowlerp_Y = true;
                if (cameraPositionY - playerPositionY > lerpDistanceYDown)
                    m_Allowlerp_Y = true;
            }

            if (m_Allowlerp_Y)
            {
                cameraPositionY = Lerp(cameraPositionY, playerPositionY, lerpSpeed);
                if (abs(cameraPositionY - playerPositionY) < lerpToleranceY)
                    m_Allowlerp_Y = false;
            }
        }

    private:
        bool m_Allowlerp_X = false;
        bool m_Allowlerp_Y = false;

        int lerpDistanceX = 7;
        int lerpDistanceYUp = 7;
        int lerpDistanceYDown = 0.2;
        float lerpToleranceX = 1.6;
        float lerpToleranceY = 0.1;
        float lerpSpeed = 0.015f;
    };
}