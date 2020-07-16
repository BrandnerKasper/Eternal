#pragma once

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
		/* Windows x64  */
		#define ET_PLATFORM_WINDOWS
	#else
		/* Windows x86 */
		#error "x86 Builds are not supported!"
	#endif
#endif

#ifdef ET_PLATFORM_WINDOWS
#if ET_DYNAMIC_LINK
	#ifdef ET_BUILD_DLL
		#define ETERNAL_API __declspec(dllexport)
	#else
		#define ETERNAL_API __declspec(dllimport)
	#endif
#else
	#define ETERNAL_API
#endif
#else
	#error Eternal is only for Windows!
#endif

/*
#ifdef ET_DEBUG
	#define	ET_ENABLE_ASSERTS
#endif
*/

#ifdef ET_ENABLE_ASSERTS
	#define ET_ASSERT(x, ...) { if(!(x)) {ET_ERROR("Assertion failed: {0}", __VA__ARGS__): __debugbreak(); } }
	#define ET_CORE_ASSERT(x, ...) { if(!(x)) {ET_CORE_ERROR("Assertion failed: {0}",  __VA__ARGS__): __debugbreak(); } }
#else
	#define ET_ASSERT(x, ...)
	#define ET_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define ET_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)