#include "ETpch.h"

#include "VertexArray.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Eternal {

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			ET_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;
			break;
		case RendererAPI::OpenGl:
			return new OpenGLVertexArray();
			break;
		default:
			ET_CORE_ASSERT(false, "Unknown RendererAPI!");
			return nullptr;
			break;
		}
	}
}