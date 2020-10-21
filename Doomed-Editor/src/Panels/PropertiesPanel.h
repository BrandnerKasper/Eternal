#pragma once

#include "Eternal.h"
#include "SceneHierarchyPanel.h"

namespace Eternal {
	
	class PropertiesPanel
	{
	public:
		PropertiesPanel() = default;
		PropertiesPanel(const Ref<SceneHierarchyPanel>& context);

		void SetContext(const Ref<SceneHierarchyPanel>& context);

		void OnImGuiRender();
	private:
		void DrawComponents(Entity entity);
	private:
		Ref<SceneHierarchyPanel> m_SceneHierachyPanel;

	};
}