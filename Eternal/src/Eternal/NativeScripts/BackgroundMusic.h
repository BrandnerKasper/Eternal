#pragma once

#include "ETpch.h"
#include "Eternal.h"

namespace Eternal {

    class BackgroundMusic : public ScriptableEntity
    {
    public:

        void OnCreate()
        {
            auto& entityname = GetComponent<TagComponent>().Tag;
            ET_INFO(entityname + " Background Music created");

            auto& audioComponent = GetComponent<AudioComponent>();
            audioComponent.SetGain();
            audioComponent.SetPitch();
            audioComponent.SetLoop();
            m_Entity.GetScene()->PlayAudioFile(audioComponent.file);
        }

        void OnDestroy()
        {
            auto& audioComponent = GetComponent<AudioComponent>();
            audioComponent.file.SetGain(0.0f);
            audioComponent.file.SetLoop(false);
        }

        void OnUpdate(Timestep ts)
        {
        }

    private:
    };
}