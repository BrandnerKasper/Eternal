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

#ifdef ET_ENABLE_ASSERTS
	#define ET_ASSERT(x, ...) { if(!(x)) {ET_ERROR("Assertion failed: {0}", __VA__ARGS__): __debugbreak(); } }
	#define ET_CORE_ASSERT(x, ...) { if(!(x)) {ET_CORE_ERROR("Assertion failed: {0}",  __VA__ARGS__): __debugbreak(); } }
#else
	#define ET_ASSERT(x, ...)
	#define ET_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)