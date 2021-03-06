#pragma once

#include "Eternal.h"

namespace Eternal {

	

	class SceneHierarchyPanel 
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const SharedPtr<Scene>& context);

		void SetContext(const SharedPtr<Scene>& context);

		void OnImGuiRender();

		Entity GetSelectedEntity() { return m_SelectedEntity; }
	private:
		void DrawSceneHierachy();
		void DrawGroupNode(Group& group);
		void DrawEntityOutsideGroupNode();
		void DrawEntityNode(Entity entity);
		void HandleRightClickOnEntity(Entity entity);
		void DeleteEntity(Entity entity);
		void HandleRightClickOnGroup();
		void DeleteGroup(Group& group);
		void HandleDrag(Entity entity);
		void HandleDrop(Group& group);
		void HandleRigthClickOnBlankSpace();
		void DeSelect();
	private:
		SharedPtr<Scene> m_Context;
		Entity m_SelectedEntity;
		bool m_entityDeleted = false;
		bool m_GroupDeleted = false;
		
		Group* m_SelectedGroup;
		friend class PropertiesPanel;
	};
}