#pragma once

#include "Eternal.h"

namespace Eternal {

	class SceneViewportPanel
	{
	public:
		SceneViewportPanel();

		void SetContext(const Ref<Scene>& context);

		void OnUpdate(Timestep ts);
		void HandleResize();
		void OnViewportResize(uint32_t width, uint32_t height);
		void UpdateScene(Timestep ts);

		void OnImGuiRender();
	private:
		Ref<Scene> m_Scene;
		Ref<FrameBuffer> m_FrameBuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		bool m_ViewportFocused = false, m_ViewportHovered = false;
	};
}