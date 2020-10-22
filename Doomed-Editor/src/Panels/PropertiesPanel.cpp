#include "PropertiesPanel.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Eternal {

	PropertiesPanel::PropertiesPanel(const Ref<SceneHierarchyPanel>& context)
	{
		SetContext(context);
	}

	void PropertiesPanel::SetContext(const Ref<SceneHierarchyPanel>& context)
	{
		m_SceneHierachyPanel = context;
	}

	void PropertiesPanel::OnImGuiRender()
	{
		ImGui::Begin("Properties Panel");
		auto& entity = m_SceneHierachyPanel->m_SelectedEntity;
		if (entity)
			DrawComponents(entity);

		ImGui::End();
	}

	//TODO: Use a templated fct to stop repeating code like if(has entity) do that etc
	void PropertiesPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			ImGui::NewLine();
			ImGui::Text("Transform");

			//z-Position should be between near and far clip of camera!
			auto& position = entity.GetComponent<TransformComponent>().Position;
			ImGui::DragFloat3("Position", glm::value_ptr(position), 0.5f);

			auto& size = entity.GetComponent<TransformComponent>().Size;
			ImGui::DragFloat2("Size", glm::value_ptr(size), 0.5f);

			auto& rotation = entity.GetComponent<TransformComponent>().Rotation;
			ImGui::DragFloat("Rotation", &rotation, 1.0f);
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			ImGui::NewLine();
			ImGui::Text("Sprite Component");

			auto& texture = entity.GetComponent<SpriteRendererComponent>().Texture;
			if (texture != nullptr)
			{
				ImGui::Image((void*)texture->GetRendererID(), ImVec2(256.0f, 256.0f), ImVec2{ 0,1 }, ImVec2{ 1,0 });

				auto& uvScale = entity.GetComponent<SpriteRendererComponent>().TextureScale;
				ImGui::SliderInt("UV Scaling", &uvScale, 1, 10);

			}

			auto& tintColor = entity.GetComponent<SpriteRendererComponent>().Color;
			ImGui::ColorEdit4("Tint Color", glm::value_ptr(tintColor));
		}

		if (entity.HasComponent<CameraComponent>())
		{
			ImGui::NewLine();
			ImGui::Text("Camera Component");

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			ImGui::Checkbox("Primary", &cameraComponent.Primary);

			float orthoSize = camera.GetOrthographicSize();
			if (ImGui::DragFloat("View Size", &orthoSize))
				camera.SetOrthographicSize(orthoSize);

			float orthoNear = camera.GetOrthographicNearClip();
			if (ImGui::DragFloat("Near Clip", &orthoNear))
				camera.SetOrthographicNearClip(orthoNear);

			float orthoFar = camera.GetOrthographicFarClip();
			if (ImGui::DragFloat("Far Clip", &orthoFar))
				camera.SetOrthographicFarClip(orthoFar);
		}
	}
}
