#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Eternal {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectedEntity = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierachy");

		m_Context->m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID , m_Context.get() };
				DrawEntityNode(entity);
			});

		HandleRigthClickOnBlankSpace();

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0)
			| ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectedEntity = entity;
		}

		HandleRightClickOnEntity();

		if (opened)
		{
			ImGui::TreePop();
		}
		//Deselect Entity -> Property panel shows nothing
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectedEntity = {};

		DeleteEntity(entity);
	
	}

	void SceneHierarchyPanel::HandleRightClickOnEntity()
	{
		//Handle Right Click on Entity
		m_entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				m_entityDeleted = true;

			ImGui::EndPopup();
		}
	}

	void SceneHierarchyPanel::DeleteEntity(Entity entity)
	{
		//Delete entity at last to not run in misbehaviour
		if (m_entityDeleted)
		{
			if (m_SelectedEntity == entity)
				m_SelectedEntity = {};
			m_Context->DestroyEntity(entity);
		}
	}

	void SceneHierarchyPanel::HandleRigthClickOnBlankSpace()
	{
		//Right Click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}
	}

}