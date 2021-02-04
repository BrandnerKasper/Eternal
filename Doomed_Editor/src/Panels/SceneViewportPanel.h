#pragma once

#include "Eternal.h"

namespace Eternal {

	class SceneViewportPanel
	{
	public:
		SceneViewportPanel();

		void SetContext(const SharedPtr<Scene>& context);

		void OnUpdate(Timestep ts);
		void HandleResize();
		void OnViewportResize(uint32_t width, uint32_t height);
		void UpdateScene(Timestep ts);
		void OnEvent(Event& event);

		void OnImGuiRender();

		float GetViewportSizeX() { return m_ViewportSize.x; }
		float GetViewportSizeY() { return m_ViewportSize.y; }
	private:
		SharedPtr<Scene> m_Scene;
		SharedPtr<FrameBuffer> m_FrameBuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		bool m_ViewportFocused = false, m_ViewportHovered = false;
	};
}