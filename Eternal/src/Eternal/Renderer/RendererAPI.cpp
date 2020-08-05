#include "ETpch.h"

#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Eternal {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGl;

	float RendererAPI::GetTime()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			ET_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return 0.0f;
			break;
		case RendererAPI::API::OpenGl:
			return OpenGLContext::OpenGLGetTime();
		default:
			ET_CORE_ASSERT(false, "Unknown RendererAPI!")
				break;
		}
	}
}