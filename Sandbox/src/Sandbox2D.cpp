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
	m_TextureScalingPtr = &m_Scale;
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

	Eternal::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Eternal::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, m_CheckerboardTexture, *m_TextureScalingPtr);
	Eternal::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.8f, 0.5f, 0.2f, 1.0f });
	Eternal::Renderer2D::DrawQuad({ 0.0f,  0.0f, -0.1f }, { 10.0f, 10.0f }, m_SquareColor);

	Eternal::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Background Square Color", glm::value_ptr(m_SquareColor));
	ImGui::SliderInt("Checkerboard UV Scaling", m_TextureScalingPtr, 1, 10);
	ImGui::End();
}

void Sandbox2D::OnEvent(Eternal::Event& event)
{
	m_CameraController.OnEvent(event);
}
