#include "ETpch.h"

#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Eternal {

	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TwoInOneShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		//test square <3
		s_Data = new Renderer2DStorage();
		s_Data->QuadVertexArray = VertexArray::Create();

		float squaredVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> squaredVertexBuffer;
		squaredVertexBuffer.reset(VertexBuffer::Create(squaredVertices, sizeof(squaredVertices)));
		BufferLayout squareLayout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"}
		};
		squaredVertexBuffer->SetLayout(squareLayout);
		s_Data->QuadVertexArray->AddVertexBuffer(squaredVertexBuffer);

		uint32_t squaredIndices[6] = { 0, 1, 2, 2, 3, 0 };
		uint32_t squareCount = sizeof(squaredIndices) / sizeof(uint32_t);
		Ref<IndexBuffer> squaredIndexBuffer;
		squaredIndexBuffer.reset(IndexBuffer::Create(squaredIndices, squareCount));
		s_Data->QuadVertexArray->SetIndexBuffer(squaredIndexBuffer);

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data->TwoInOneShader = Shader::Create("assets/shaders/2in1TextureAndColor.glsl");
		s_Data->TwoInOneShader->Bind();
		s_Data->TwoInOneShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->TwoInOneShader->Bind();
		s_Data->TwoInOneShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data->TwoInOneShader->SetFloat4("u_Color", color);
		s_Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TwoInOneShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const int textureScale, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, textureScale, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const int textureScale, const glm::vec4& tintColor)
	{
		s_Data->TwoInOneShader->SetFloat4("u_Color", tintColor);

		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TwoInOneShader->SetMat4("u_Transform", transform);

		texture->Bind();

		s_Data->TwoInOneShader->SetInt("u_TexScale", textureScale);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0 }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		s_Data->TwoInOneShader->SetFloat4("u_Color", color);
		s_Data->WhiteTexture->Bind();

		glm::mat4 stdMat4 = glm::mat4(1.0f);
		glm::mat4 transform = glm::translate(stdMat4, position)
			* glm::rotate(stdMat4, rotation, {0.0f, 0.0f, 1.0f})
			* glm::scale(stdMat4, { size.x, size.y, 1.0f });
		s_Data->TwoInOneShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const int textureScale, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0 }, size, rotation, texture, textureScale, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const int textureScale, const glm::vec4& tintColor)
	{
		s_Data->TwoInOneShader->SetFloat4("u_Color", tintColor);

		glm::mat4 stdMat4 = glm::mat4(1.0f);
		glm::mat4 transform = glm::translate(stdMat4, position)
			* glm::rotate(stdMat4, rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(stdMat4, { size.x, size.y, 1.0f });
		s_Data->TwoInOneShader->SetMat4("u_Transform", transform);

		texture->Bind();

		s_Data->TwoInOneShader->SetInt("u_TexScale", textureScale);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}