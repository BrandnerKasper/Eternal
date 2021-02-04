#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Eternal {

	SceneHierarchyPanel::SceneHierarchyPanel(const SharedPtr<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const SharedPtr<Scene>& context)
	{
		m_Context = context;
		m_SelectedEntity = {};
		m_SelectedGroup = nullptr;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierachy");
		
		DrawSceneHierachy();
		HandleRigthClickOnBlankSpace(); // ->Make this into a Button
		DeSelect();

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawSceneHierachy()
	{
		if (m_Context->m_Groups.size() != 0)
		{
			for (auto& group : m_Context->m_Groups)
			{
				DrawGroupNode(group);
			}
			DrawEntityOutsideGroupNode();
		}
		else
		{
			m_Context->m_Registry.each([&](auto entityID)
				{
					Entity entity{ entityID , m_Context.get() };
					DrawEntityNode(entity);
				});
		}
	}

	void SceneHierarchyPanel::DrawGroupNode(Group& group)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
		if (m_SelectedGroup)
			flags = ((m_SelectedGroup->m_ID == group.m_ID) ? ImGuiTreeNodeFlags_Selected : 0);
		if (group.m_ID != 0)
		{
			auto TreeGroup = ImGui::TreeNodeEx(group.m_Name.c_str(), flags);
			if (ImGui::IsItemClicked())
			{
				m_SelectedGroup = &group;
				m_SelectedEntity = {};
			}

			HandleDrop(group);

			if (TreeGroup)
			{
				//Draw Entitys inside Group
				m_Context->m_Registry.each([&](auto entityID)
					{
						Entity entity{ entityID , m_Context.get() };
						auto entityGroupID = entity.GetComponent<TagComponent>().Group_ID;
						if (group.m_ID == entityGroupID)
						{
							DrawEntityNode(entity);
						}
					});
				ImGui::TreePop();
			}
			

			HandleRightClickOnGroup();
			DeleteGroup(group);
		}
	}

	void SceneHierarchyPanel::DrawEntityOutsideGroupNode()
	{
		//Draw Entitys outside Group
		m_Context->m_Registry.each([&](auto entityID)
			{
				bool drawMe = true;
				Entity entity{ entityID , m_Context.get() };
				auto entityGroupID = entity.GetComponent<TagComponent>().Group_ID;
				for each (Group group in m_Context->m_Groups)
				{
					if (group.m_ID == entityGroupID)
					{
						drawMe = false;
					}
				}
				if (drawMe)
					DrawEntityNode(entity);
			});
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0);
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		flags |= ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		ImGui::TreeNodeEx((void*)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectedEntity = entity;
			m_SelectedGroup = {};
		}

		HandleDrag(entity);
		
		HandleRightClickOnEntity(entity);

		DeleteEntity(entity);
	
	}

	void SceneHierarchyPanel::HandleRightClickOnEntity(Entity entity) //make Button for that!
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

	void SceneHierarchyPanel::HandleRightClickOnGroup()
	{
		//Handle Right Click on Group
		m_GroupDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Group"))
				m_GroupDeleted = true;

			ImGui::EndPopup();
		}
	}

	void SceneHierarchyPanel::DeleteGroup(Group& group)
	{
		//Delete entity at last to not run in misbehaviour
		if (m_GroupDeleted)
		{
			if (m_SelectedGroup)
			{
				if (m_SelectedGroup->m_ID == group.m_ID)
					m_SelectedGroup = {};
			}
			m_Context->DestroyGroup(group);
		}
	}

	void SceneHierarchyPanel::HandleDrag(Entity entity)
	{
		//HandleDragAndDrop --- Part 1 Drag...
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			auto entityID = entity.GetEntityID();
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			// Set payload to carry the index of our item (could be anything)
			ImGui::SetDragDropPayload("DragEntityIntoGroup", &entityID, sizeof(int));
			// Display preview
			ImGui::Text("Move %s", tag.c_str());
			ImGui::EndDragDropSource();
		}
	}

	void SceneHierarchyPanel::HandleDrop(Group& group)
	{
		//HandleDragAndDrop --- Part 2 ...Drop
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DragEntityIntoGroup"))
			{
				ET_ASSERT(payload->DataSize == sizeof(int), "The dragged Payload does not contain the right data!");

				int payload_n = *(const int*)payload->Data;

				m_Context->m_Registry.each([&](auto entityID)
					{
						Entity entity{ entityID , m_Context.get() };
						auto& entityGroupID = entity.GetComponent<TagComponent>().Group_ID;
						if ((int)entityID == (int)payload_n)
							entityGroupID = group.m_ID;
					});

				ImGui::EndDragDropTarget();
			}
		}
	}

	void SceneHierarchyPanel::HandleRigthClickOnBlankSpace()
	{
		//Right Click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			m_SelectedGroup = nullptr; //TODO
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");
			if (ImGui::MenuItem("Create Empty Group"))
			{
				m_Context->CreateGroup("Nameless Group");
			}
			ImGui::EndPopup();
		}
	}

	void SceneHierarchyPanel::DeSelect()
	{
		//Deselect Entity or Group -> Property panel shows nothing
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_SelectedEntity = {};
			m_SelectedGroup = nullptr;
		}
	}

}