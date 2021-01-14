#include "ETpch.h"
#include "AudioManager.h"

namespace Eternal {

	void AudioManager::Init()
	{
		Hazel::Audio::Init();
	}

	void AudioManager::Play(const Hazel::AudioSource& source)
	{
		Hazel::Audio::Play(source);
	}

}