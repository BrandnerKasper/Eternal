#pragma once

#include "ETpch.h"
#include "Eternal.h"

namespace Eternal {

    class Rotater : public ScriptableEntity
    {
    public:
        void OnCreate()
        {
            auto& entityname = GetComponent<TagComponent>().Tag;
            ET_INFO(entityname + " Rotate created");
        }

        void OnDestroy()
        {

        }

        void OnUpdate(Timestep ts)
        {
            auto& rotation = GetComponent<TransformComponent>().Rotation;
            rotation += 10 * ts;

        }
    };
}