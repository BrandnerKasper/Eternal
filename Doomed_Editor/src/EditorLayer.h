#pragma once

#include <Eternal.h>
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/PropertiesPanel.h"
#include "Panels/SettingsPanel.h"
#include "Panels/SceneViewportPanel.h"
#include "box2d/include/box2d/box2d.h"

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
		
	private:
		void ChangeSytle(bool show);
		bool OnKeyPressed(KeyPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveSceneAs();

		void Exit();
	private:
		Ref<Scene> m_ActiveScene;
		
		//Panel
		Ref<SceneHierarchyPanel> m_SceneHierachyPanel;
		Ref<PropertiesPanel> m_PropertiesPanel;
		Ref<SettingsPanel> m_SettingsPanel;
		Ref<SceneViewportPanel> m_SceneViewportPanel;

		float m_physicsTime = 1.0f / 60.0f;
	};
}