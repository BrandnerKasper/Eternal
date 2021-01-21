#pragma once

#include "ETpch.h"
#include "Eternal.h"

namespace Eternal {

    class Load1_1 : public ScriptableEntity
    {
    public:
        void OnCreate()
        {
            auto& entityname = GetComponent<TagComponent>().Tag;
            ET_INFO(entityname + " Load1_1 created");
        }

        void OnDestroy()
        {

        }

        void OnUpdate(Timestep ts)
        {

        }

        void OnEvent(Event& e)
        {
            EventDispatcher dispatcher(e);
            dispatcher.Dispatch<KeyPressedEvent>(ET_BIND_EVENT_FN(Load1_1::HandleKeyPressedEvent));
        }

        bool HandleKeyPressedEvent(KeyPressedEvent& e)
        {
            if (e.GetRepeatCount() > 0)
                return false;

            m_Entity.GetScene()->SetPlay(false);

            m_Entity.GetScene()->SetChangeScene(true);
            m_Entity.GetScene()->SetChangeSceneFilepath("assets/scenes/level01.eternal");
        }


    };
}