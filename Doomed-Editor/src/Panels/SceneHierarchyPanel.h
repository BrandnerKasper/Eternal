#pragma once

#include "Eternal.h"

namespace Eternal {

	class SceneHierarchyPanel 
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();

		Entity GetSelectedEntity() { return m_SelectedEntity; }
	private:
		void DrawEntityNode(Entity entity);
		void HandleRightClickOnEntity();
		void DeleteEntity(Entity entity);
		void HandleRigthClickOnBlankSpace();
	private:
		Ref<Scene> m_Context;
		Entity m_SelectedEntity;
		bool m_entityDeleted = false;

		friend class PropertiesPanel;
	};
}