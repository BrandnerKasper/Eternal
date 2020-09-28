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
	m_CheckerboardTexture = Eternal::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Eternal::Timestep ts) // Max Frame Rate auf 60 cappen maybe!!
{
	//Update
	m_CameraController.OnUpdate(ts);

	//Renderer Stats
	Eternal::Renderer2D::ResetStats();

	//Render
	Eternal::RenderCommand::SetClearColor({ 0.7f, 0.7f, 0.7f, 1 });
	Eternal::RenderCommand::Clear();

	Eternal::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Eternal::Renderer2D::DrawRotatedQuad(m_Position, m_Scale, m_Rotation, m_CheckerboardTexture, m_Tiling, m_TintColor);
	Eternal::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.8f, 0.5f, 0.2f, 1.0f });
	Eternal::Renderer2D::DrawQuad({ 0.0f,  0.0f, -0.1f }, { 10.0f, 10.0f }, m_SquareColor);
	Eternal::Renderer2D::DrawRotatedQuad({ 1.0f, 1.0f, 0.0f }, { 2.0f, 0.5f }, 20.0f, { 0.3f, 0.8f, 0.4f, 1.0f });
	Eternal::Renderer2D::DrawRotatedQuad({ 3.0f, 4.0f, 0.0f }, { 2.0f,2.0f }, 35.0f, Eternal::Texture2D::Create("assets/textures/EternalLogo.png"));

	Eternal::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");

	auto stats = Eternal::Renderer2D::GetStats();
	ImGui::Text("Batch Renderer Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quad Count: %d", stats.QuadCount);
	ImGui::Text("Vertex Count: %d", stats.GetTotalVertexCount());
	ImGui::Text("Index Count: %d", stats.GetTotalIndexCount());

	ImGui::Text("Chess Quad:");
	ImGui::SliderFloat3("Checkerboard Position", glm::value_ptr(m_Position), -10.0f, 10.0f);
	ImGui::SliderFloat2("Checkerboard Quad Scale", glm::value_ptr(m_Scale), 0.0f, 10.0f);
	ImGui::ColorEdit4("Background Square Color", glm::value_ptr(m_SquareColor));
	ImGui::SliderInt("Checkerboard UV Scaling", &m_Tiling, 1, 10);
	ImGui::SliderFloat("Checkerboard Rotation", &m_Rotation, 0.0f, 360.0f);
	ImGui::ColorEdit4("Tint Color of Checkerboard", glm::value_ptr(m_TintColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Eternal::Event& event)
{
	m_CameraController.OnEvent(event);
}
