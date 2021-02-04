#pragma once

#include "Eternal.h"
#include "SceneHierarchyPanel.h"

namespace Eternal {
	
	class PropertiesPanel
	{
	public:
		PropertiesPanel() = default;
		PropertiesPanel(const SharedPtr<SceneHierarchyPanel>& context);

		void SetContext(const SharedPtr<SceneHierarchyPanel>& context);

		void OnImGuiRender();
	private:
		void DrawComponents(Entity entity);
		void DrawGroup(Group* group);

		template<typename T, typename UIFunction>
		static void HandleComponent(const std::string& name, Entity entity, UIFunction uiFunction, bool showRemove = true);		
		static void HandleAddComponentButton(Entity entity);
	private:
		SharedPtr<SceneHierarchyPanel> m_SceneHierachyPanel;
	};
}