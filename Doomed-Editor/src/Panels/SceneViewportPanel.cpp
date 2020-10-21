#include "SceneViewportPanel.h"

#include <imgui/imgui.h>

namespace Eternal {

	SceneViewportPanel::SceneViewportPanel()
	{
		FrameBufferSpecification frameBufferSpec;
		frameBufferSpec.Width = 1280;
		frameBufferSpec.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(frameBufferSpec);
		
		//SetContext(context);
	}

	void SceneViewportPanel::SetContext(const Ref<Scene>& context)
	{
		m_Scene = context;
	}

	void SceneViewportPanel::OnUpdate(Timestep ts)
	{
		HandleResize();
		
		UpdateScene(ts);
	}

	void SceneViewportPanel::HandleResize()
	{
		FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
		if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

			m_Scene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}
	}

	void SceneViewportPanel::UpdateScene(Timestep ts)
	{
		//Renderer Stats
		Renderer2D::ResetStats();

		m_FrameBuffer->Bind();

		//Render Clear
		RenderCommand::SetClearColor({ 0.7f, 0.7f, 0.7f, 1 });
		RenderCommand::Clear();

		//Update Scene!!!!!!!!!!!!!!!!!!!!!!!!
		m_Scene->OnUpdate(ts);

		m_FrameBuffer->Unbind();
	}

	void SceneViewportPanel::OnImGuiRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });

		ImGui::Begin("Scene Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*) & viewportPanelSize))
		{
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}
		uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

		ImGui::End();

		ImGui::PopStyleVar();
	}


}