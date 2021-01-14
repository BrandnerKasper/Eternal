#pragma once

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <HazelAudio.h>
#pragma warning(pop)

namespace Eternal {

	class AudioManager
	{
	public:
		static void Init();
		static void Play(const Hazel::AudioSource& source);
	};
}