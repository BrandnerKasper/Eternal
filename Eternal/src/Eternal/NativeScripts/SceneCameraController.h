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
            lerpDistanceX = GetComponent<CameraComponent>().Camera.GetOrthographicSize() / 2 + 1;
            lerpDistanceY = GetComponent<CameraComponent>().Camera.GetOrthographicSize() / 4 + 2;
        }

        void OnDestroy()
        {
        }

        void OnUpdate(Timestep ts)
        {
            HandleLerp();
        }

        float lerp(float a, float b, float f)
        {
            return a + f * (b - a);
        }

        void HandleLerp()
        {
            auto& cameraPosition = GetComponent<TransformComponent>().Position;
            
            auto& cameraPositionX = cameraPosition.x;
            auto& cameraPositionY = cameraPosition.y;

            auto& playerPosition = m_Entity.GetScene()->GetEntity("Player").GetComponent<TransformComponent>().Position;

            auto& playerPositionX = playerPosition.x;
            auto& playerPositionY = playerPosition.y;

            HandleLerpX(cameraPositionX, playerPositionX);

            HandleLerpY(cameraPositionY, playerPositionY);
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
                cameraPositionX = lerp(cameraPositionX, playerPositionX, lerpSpeed);
                if (abs(cameraPositionX - playerPositionX) < lerpTolerance)
                    m_Allowlerp_X = false;
            }
        }

        void HandleLerpY(float& cameraPositionY, float& playerPositionY)
        {
            if (!m_Allowlerp_Y)
            {
                if (abs(cameraPositionY - playerPositionY) > lerpDistanceY)
                    m_Allowlerp_Y = true;
            }

            if (m_Allowlerp_Y)
            {
                cameraPositionY = lerp(cameraPositionY, playerPositionY, lerpSpeed);
                if (abs(cameraPositionY - playerPositionY) < lerpTolerance)
                    m_Allowlerp_Y = false;
            }
        }

    private:
        bool m_Allowlerp_X = false;
        bool m_Allowlerp_Y = false;

        int lerpDistanceX = 13;
        int lerpDistanceY = 8;
        int lerpTolerance = 3;
        float lerpSpeed = 0.03f;
    };
}