#pragma once

#include <Eternal.h>
#include "Panels/SceneHierarchyPanel.h"

namespace Eternal {
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Eternal::Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Eternal::Event& event) override;
		void ChangeSytle(bool show);
	private:
		OrthographicCameraController m_CameraController;

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		Ref<Scene> m_ActiveScene;
		Entity m_ChessSquareEntity;
		Entity m_CameraEntity;
		
		//Panel
		SceneHierarchyPanel m_SceneHierachyPanel;

		//Temporary!!
		Ref<FrameBuffer> m_FrameBuffer;
		Ref<Texture2D> m_CheckerboardTexture;
		Ref<Texture2D> m_DoomTexture;
		glm::vec4 m_TintColor = { 1.0f, 1.0f, 0.8f, 1.0f };

	};
}