#pragma once

#include "ETpch.h"
#include "Eternal.h"

namespace Eternal {

    class CameraController : public ScriptableEntity
    {
    public:
        void OnCreate()
        {
            //std::cout << "i am alive.";
        }

        void OnDestroy()
        {

        }

        void OnUpdate(Timestep ts)
        {
            auto& position = GetComponent<TransformComponent>().Position;
            float speed = 10.0f;
            if (Input::IsKeyPressed(ET_KEY_A))
                position.x -= speed * ts;
            if (Input::IsKeyPressed(ET_KEY_D))
                position.x += speed * ts;
            if (Input::IsKeyPressed(ET_KEY_W))
                position.y += speed * ts;
            if (Input::IsKeyPressed(ET_KEY_S))
                position.y -= speed * ts;
        }
    };
}