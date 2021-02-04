#include "ETpch.h"

#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Eternal {

	SharedPtr<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ET_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGl:
			return CreateSharedPtr<OpenGLVertexBuffer>(size);
			break;
		default:
			ET_CORE_ASSERT(false, "Unknown RendererAPI!");
			return nullptr;
			break;
		}
	}

	SharedPtr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ET_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGl:
			return CreateSharedPtr<OpenGLVertexBuffer>(vertices, size);
			break;
		default:
			ET_CORE_ASSERT(false, "Unknown RendererAPI!");
			return nullptr;
			break;
		}
	}

	SharedPtr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ET_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGl:
			return CreateSharedPtr<OpenGLIndexBuffer>(indices, size);
			break;
		default:
			ET_CORE_ASSERT(false, "Unknown RendererAPI!");
			return nullptr;
			break;
		}
	}

}