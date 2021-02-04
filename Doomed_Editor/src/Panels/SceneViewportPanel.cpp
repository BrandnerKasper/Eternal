#include "SceneViewportPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Eternal {

	SceneViewportPanel::SceneViewportPanel()
	{
		FrameBufferSpecification frameBufferSpec;
		frameBufferSpec.Width = 1280;
		frameBufferSpec.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(frameBufferSpec);
	}

	void SceneViewportPanel::SetContext(const SharedPtr<Scene>& context)
	{
		m_Scene = context;

		m_Scene->m_EditorCamera = CreateSharedPtr<EditorCameraController>(1.778f);

	}

	void SceneViewportPanel::OnUpdate(Timestep ts)
	{
		HandleResize();

		//Update Camera only when Window is focused
		m_Scene->m_SceneFocused = m_ViewportFocused;

		UpdateScene(ts);
	}

	void SceneViewportPanel::HandleResize()
	{
		FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
		if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

			OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}
	}

	void SceneViewportPanel::OnViewportResize(uint32_t width, uint32_t height)
	{
		ET_CORE_ERROR("Viewport Resized width, height: {0}, {1}", width, height);
		m_Scene->OnViewportResize(width, height);
	}

	void SceneViewportPanel::UpdateScene(Timestep ts)
	{
		//Renderer Stats
		Renderer2D::ResetStats();

		m_FrameBuffer->Bind();

		//Render Clear
		RenderCommand::SetClearColor({ 0.839, 1, 0.917, 0.8 });
		RenderCommand::Clear();

		//Update Scene!!!!!!!!!!!!!!!!!!!!!!!!
		m_Scene->OnUpdate(ts);

		m_FrameBuffer->Unbind();
	}

	void SceneViewportPanel::OnEvent(Event& event)
	{
		m_Scene->OnEvent(event);
		
		if (m_ViewportFocused)
		{
			//Editor Camera Events 
			m_Scene->m_EditorCamera->OnEvent(event);
		}
	}

	static void DrawPlay(bool& play)
	{
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 18.0f, lineHeight };

		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		//ImGui::Checkbox("Play", &play);
		
		ImGui::PushFont(boldFont);
		auto playText = "Play"; //Maybe use unicode icon for play
		auto stopText = "Stop";
		auto buttonText = "";
		if (play == false)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			buttonText = playText;
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			buttonText = stopText;
		}
		if (ImGui::Button(buttonText, buttonSize))
		{
			if (play == false)
				play = true;
			else
				play = false;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
	}

	void SceneViewportPanel::OnImGuiRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,3 });
		ImGui::Begin("Scene Viewport");

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration;
		ImGui::BeginChild("Play Button", ImVec2(m_ViewportSize.x, 25),false, flags);

		
		ImGui::SameLine(m_ViewportSize.x / 2, 20.0f);
		auto& play = m_Scene->m_play;
		DrawPlay(play);
		
		ImGui::EndChild();

		ImGui::BeginChild("Scene");
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer().BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*) & viewportPanelSize))
		{
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}
		uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

		ImGui::EndChild();

		ImGui::End();

		ImGui::PopStyleVar();
	}
}