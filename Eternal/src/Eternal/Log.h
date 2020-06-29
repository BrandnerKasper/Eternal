#pragma once

#include <memory>
#include "Core.h"
#include "spdlog\spdlog.h"

namespace Eternal {

	class ETERNAL_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
		
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

//Core Log Macros (descending order)
#define ET_CORE_FATAL(...)  ::Eternal::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#define ET_CORE_ERROR(...)  ::Eternal::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ET_CORE_WARN(...)   ::Eternal::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ET_CORE_INFO(...)   ::Eternal::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ET_CORE_TRACE(...)  ::Eternal::Log::GetCoreLogger()->trace(__VA_ARGS__)

//Client Log Macros (descending order)
#define ET_FATAL(...)       ::Eternal::Log::GetClientLogger()->fatal(__VA_ARGS__)
#define ET_ERROR(...)       ::Eternal::Log::GetClientLogger()->error(__VA_ARGS__)
#define ET_WARN(...)        ::Eternal::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ET_INFO(...)        ::Eternal::Log::GetClientLogger()->info(__VA_ARGS__)
#define ET_TRACE(...)       ::Eternal::Log::GetClientLogger()->trace(__VA_ARGS__)

