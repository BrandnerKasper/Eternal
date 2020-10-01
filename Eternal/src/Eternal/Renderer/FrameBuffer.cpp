#include "ETpch.h"
#include "FrameBuffer.h"

#include "Platform/OpenGL/OpenGLFrameBuffer.h"
#include "Eternal/Renderer/Renderer.h"

namespace Eternal {

	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ET_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGl:
			return CreateRef<OpenGLFramebuffer>(spec);
			break;
		default:
			ET_CORE_ASSERT(false, "Unknown RendererAPI!");
			return nullptr;
			break;
		}
	}
}