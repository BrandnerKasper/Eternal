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
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierachy");

		auto view = m_Context ->m_Registry.view<TagComponent>();
		for (auto entityID : view)
		{
			Entity entity{ entityID, m_Context.get() };
			DrawEntityNode(entity);
		}

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
		if (opened)
		{
			ImGui::TreePop();
		}
	}

}