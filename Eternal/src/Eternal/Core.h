#pragma once

#ifdef ET_PLATFORM_WINDOWS
	#ifdef ET_BUILD_DLL
		#define ETERNAL_API __declspec(dllexport)
	#else
		#define ETERNAL_API __declspec(dllimport)

	#endif
#else
	#error Eternal is only for Windows!
#endif