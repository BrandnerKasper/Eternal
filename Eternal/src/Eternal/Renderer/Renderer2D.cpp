#include "ETpch.h"

#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

namespace Eternal {

	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		//test square <3
		s_Data = new Renderer2DStorage();
		s_Data->QuadVertexArray = VertexArray::Create();

		float squaredVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
		};

		Ref<VertexBuffer> squaredVertexBuffer;
		squaredVertexBuffer.reset(VertexBuffer::Create(squaredVertices, sizeof(squaredVertices)));
		BufferLayout squareLayout = {
			{ShaderDataType::Float3, "a_Position"},
		};
		squaredVertexBuffer->SetLayout(squareLayout);
		s_Data->QuadVertexArray->AddVertexBuffer(squaredVertexBuffer);

		uint32_t squaredIndices[6] = { 0, 1, 2, 2, 3, 0 };
		uint32_t squareCount = sizeof(squaredIndices) / sizeof(uint32_t);
		Ref<IndexBuffer> squaredIndexBuffer;
		squaredIndexBuffer.reset(IndexBuffer::Create(squaredIndices, squareCount));
		s_Data->QuadVertexArray->SetIndexBuffer(squaredIndexBuffer);

		s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		s_Data->FlatColorShader->UploadUniformMat4("u_Transform", glm::mat4(1.0f));
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
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->UploadUniformFloat4("u_Color", color);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}