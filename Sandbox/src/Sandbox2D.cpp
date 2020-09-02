#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/type_ptr.hpp>
#include "Eternal/Core/EntryPoint.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	//test square <3
	m_SquareVertexArray = Eternal::VertexArray::Create();

	float squaredVertices[3 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
	};

	Eternal::Ref<Eternal::VertexBuffer> squaredVertexBuffer;
	squaredVertexBuffer.reset(Eternal::VertexBuffer::Create(squaredVertices, sizeof(squaredVertices)));
	Eternal::BufferLayout squareLayout = {
		{Eternal::ShaderDataType::Float3, "a_Position"},
	};
	squaredVertexBuffer->SetLayout(squareLayout);
	m_SquareVertexArray->AddVertexBuffer(squaredVertexBuffer);

	uint32_t squaredIndices[6] = { 0, 1, 2, 2, 3, 0 };
	uint32_t squareCount = sizeof(squaredIndices) / sizeof(uint32_t);
	Eternal::Ref<Eternal::IndexBuffer> squaredIndexBuffer;
	squaredIndexBuffer.reset(Eternal::IndexBuffer::Create(squaredIndices, squareCount));
	m_SquareVertexArray->SetIndexBuffer(squaredIndexBuffer);

	m_FlatColorShader = Eternal::Shader::Create("assets/shaders/FlatColor.glsl");

}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Eternal::Timestep ts)
{
	//Update
	m_CameraController.OnUpdate(ts);

	//Render
	Eternal::RenderCommand::SetClearColor({ 0.7f, 0.7f, 0.7f, 1 });
	Eternal::RenderCommand::Clear();

	Eternal::Renderer::BeginScene(m_CameraController.GetCamera());

	m_FlatColorShader->Bind();
	m_FlatColorShader->UploadUniformFloat4("u_Color", m_SquareColor);

	glm::mat4 scaletex = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
	glm::vec3 postex(-2.25f, -2.25f, 0.0f);
	glm::mat4 transformtex = glm::translate(glm::mat4(1.0f), postex) * scaletex;

	Eternal::Renderer::Submit(m_FlatColorShader, m_SquareVertexArray, transformtex);

	Eternal::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Eternal::Event& event)
{
	m_CameraController.OnEvent(event);
}
