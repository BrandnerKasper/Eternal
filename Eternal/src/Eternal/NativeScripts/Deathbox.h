#pragma once

#include "ETpch.h"
#include "Eternal.h"
#include "Eternal/Physics/ContactListener.h"

namespace Eternal {

    class Deathbox : public ScriptableEntity
    {
    public:

        void OnCreate()
        {
            auto& entityname = GetComponent<TagComponent>().Tag;
            ET_INFO(entityname + " Deathbox created");

            //Set ScriptType for ContactHandler
            m_ScriptType = ScriptType::deathbox;

            //Set User Data in Physics
            auto& deathboxBody = GetComponent<PhysicsComponent>().body;
            deathboxBody->GetUserData().pointer = (uintptr_t)this;
            
            //Set Physics Component as Sensor
            GetComponent<PhysicsComponent>().Fixture->SetSensor(true);

        }

        void OnDestroy()
        {
        }

        void OnUpdate(Timestep ts)
        {
        }

        void HandleContact() override
        {
            m_Entity.GetScene()->SetPlay(false);
        }

    private:
        //b2Body* m_DeathboxBody;
    };
}