#pragma once

#include "ETpch.h"
#include "Eternal.h"

namespace Eternal {

    class CloseApp : public ScriptableEntity
    {
    public:
        void OnCreate()
        {
            auto& entityname = GetComponent<TagComponent>().Tag;
            ET_INFO(entityname + " Close App created");
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
            dispatcher.Dispatch<KeyPressedEvent>(ET_BIND_EVENT_FN(CloseApp::HandleKeyPressedEvent));
        }

        bool HandleKeyPressedEvent(KeyPressedEvent& e)
        {
            if (e.GetRepeatCount() > 0)
                return false;

            Application::Get().Close();
        }


    };
}