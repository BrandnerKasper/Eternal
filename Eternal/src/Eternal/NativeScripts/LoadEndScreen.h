#pragma once

#include "ETpch.h"
#include "Eternal.h"
#include "Eternal/Physics/ContactListener.h"

namespace Eternal {

    class LoadEndScreen : public ScriptableEntity
    {
    public:

        void OnCreate()
        {
            auto& entityname = GetComponent<TagComponent>().Tag;
            ET_INFO(entityname + " Load EndScreen created");

            //Set ScriptType for ContactHandler
            m_ScriptType = ScriptType::scenetransistion;

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

        void HandlePlayerEntered()
        {
            m_Entity.GetScene()->SetPlay(false);

            m_Entity.GetScene()->SetChangeScene(true);
            m_Entity.GetScene()->SetChangeSceneFilepath("assets/scenes/EndScreen.eternal");
        }

    private:
    };
}