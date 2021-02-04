#include "ETpch.h"

#include "Shader.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Eternal {

	SharedPtr<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ET_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGl:
			return std::make_shared<OpenGLShader>(filepath);
		default:
			ET_CORE_ASSERT(false, "Unknown RendererAPI!");
			return nullptr;
			break;
		}
	}

	SharedPtr<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ET_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;
			break;
		case RendererAPI::API::OpenGl:
			return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		default:
			ET_CORE_ASSERT(false, "Unknown RendererAPI!");
			return nullptr;
			break;
		}
	}

	void ShaderLibrary::Add(const std::string& name, const SharedPtr<Shader>& shader)
	{
		ET_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const SharedPtr<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}


	SharedPtr<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	SharedPtr<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	SharedPtr<Shader> ShaderLibrary::Get(const std::string& name)
	{
		ET_CORE_ASSERT(Exists(name), "Shader not found!");
		return m_Shaders[name];
	}
	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}