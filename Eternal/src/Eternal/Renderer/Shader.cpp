#include "ETpch.h"

#include "Shader.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Eternal {

	Shader* Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ET_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGl:
			return new OpenGLShader(filepath);
		default:
			ET_CORE_ASSERT(false, "Unknown RendererAPI!");
			return nullptr;
			break;
		}
	}

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ET_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGl:
			return new OpenGLShader(vertexSrc, fragmentSrc);
		default:
			ET_CORE_ASSERT(false, "Unknown RendererAPI!");
			return nullptr;
			break;
		}
	}
}