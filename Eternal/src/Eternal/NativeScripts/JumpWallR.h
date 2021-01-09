#pragma once

#include "ETpch.h"
#include "Eternal.h"
#include "Eternal/Physics/ContactListener.h"

namespace Eternal {

    class JumpWallR : public ScriptableEntity
    {
    public:

        void OnCreate()
        {
            auto& entityname = GetComponent<TagComponent>().Tag;
            ET_INFO(entityname + " JumpWall created");

            //Set ScriptType for ContactHandler
            m_ScriptType = ScriptType::jumpwallR;

            //Set User Data in Physics
            auto& jumpWallBody = GetComponent<PhysicsComponent>().body;
            jumpWallBody->GetUserData().pointer = (uintptr_t)this;

        }

        void OnDestroy()
        {
        }

        void OnUpdate(Timestep ts)
        {
        }

    private:
    };
}