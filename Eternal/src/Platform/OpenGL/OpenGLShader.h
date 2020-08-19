#pragma once

#include "Eternal/Renderer/Shader.h"

#include <glad/glad.h>

namespace Eternal {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadUniformInt(const std::string& name, int value);
		virtual void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);

		virtual void UploadUniformFloat(const std::string& name, float value);
		virtual void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		virtual void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& values);

		virtual void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string>& shaderSources);
	private:
		uint32_t m_RendererID;
	};

}