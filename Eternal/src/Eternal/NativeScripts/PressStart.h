#pragma once

#include "ETpch.h"
#include "Eternal.h"

namespace Eternal {

    class PressStart : public ScriptableEntity
    {
    public:
        void OnCreate()
        {
            auto& entityname = GetComponent<TagComponent>().Tag;
            ET_INFO(entityname + " Press Start created");
        }

        void OnDestroy()
        {

        }

        void OnUpdate(Timestep ts)
        {
            auto& positionZ = GetComponent<TransformComponent>().Position.z;
            if (positionZ <= -7.0)
                moveF = true;
            if (positionZ >= 7.0)
                moveF = false;

            if (moveF)
                positionZ += 0.05;
            else
                positionZ -= 0.05;

        }
    private:
        bool moveF = true;
    };
}