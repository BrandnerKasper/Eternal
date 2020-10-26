#include "PropertiesPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
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

	void PropertiesPanel::DrawVecControl(const std::string& label, float& value, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.7f, 0.83f, 0.42f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.8f, 0.93f, 0.52f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.7f, 0.83f, 0.42f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("R", buttonSize))
			value = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##R", &value, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	void PropertiesPanel::DrawVec2Control(const std::string& label, glm::vec2& values, float resetValue = 1.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.84f, 0.24f, 0.75f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.94f, 0.34f, 0.85f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.84f, 0.24f, 0.75f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("W", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##W", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.15f, 0.75f, 0.84f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.25f, 0.85f, 0.94f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.15f, 0.75f, 0.84f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("H", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##H", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

	}

	void PropertiesPanel::DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	//TODO: Use a templated fct to stop repeating code like if(has entity) do that etc
	void PropertiesPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tagComponent = entity.GetComponent<TagComponent>();
			DrawTagComponent(tagComponent);
		}

		if (entity.HasComponent<TransformComponent>())
		{
			//z-Position should be between near and far clip of camera!
			auto& transformComponent = entity.GetComponent<TransformComponent>();
			DrawTransformComponent(transformComponent);
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
			DrawSpriteRenderComponent(spriteRendererComponent);
		}

		if (entity.HasComponent<CameraComponent>())
		{
			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			DrawCameraComponent(cameraComponent);
		}
	}

	void PropertiesPanel::DrawTagComponent(TagComponent& tagComponent)
	{
		ImGui::Text("Tag");

		ImGui::Columns(2);
		float columnWidth = 100.0f;
		ImGui::SetColumnWidth(0, columnWidth);

		auto& tag = tagComponent.Tag;
		ImGui::Text("Name");

		ImGui::NextColumn();
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), tag.c_str());
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
		{
			tag = std::string(buffer);
		}

		ImGui::Columns(1);
	}

	void PropertiesPanel::DrawTransformComponent(TransformComponent& transformComponent)
	{
		ImGui::NewLine();
		ImGui::Text("Transform");

		//z-Position should be between near and far clip of camera!
		auto& position = transformComponent.Position;
		DrawVec3Control("Position", position);

		auto& size = transformComponent.Size;
		DrawVec2Control("Scale", size);

		auto& rotation = transformComponent.Rotation;
		DrawVecControl("Rotation", rotation);
	}

	void PropertiesPanel::DrawSpriteRenderComponent(SpriteRendererComponent& spriteRenderComponent)
	{
		ImGui::NewLine();
		ImGui::Text("Sprite properties");

		auto& texture = spriteRenderComponent.Texture;
		if (texture != nullptr)
		{
			ImGui::Image((void*)texture->GetRendererID(), ImVec2(256.0f, 256.0f), ImVec2{ 0,1 }, ImVec2{ 1,0 });

			ImGui::Columns(2);
			float columnWidth = 100.0f;
			ImGui::SetColumnWidth(0, columnWidth);
			ImGui::Text("UV Scaling");

			ImGui::NextColumn();
			auto& uvScale = spriteRenderComponent.TextureScale;
			ImGui::SliderInt("##UV Scaling", &uvScale, 1, 10);
			ImGui::Columns(1);

		}

		ImGui::Columns(2);
		float columnWidth = 100.0f;
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text("Tint Color");

		ImGui::NextColumn();
		auto& tintColor = spriteRenderComponent.Color;
		ImGui::ColorEdit4("##Tint Color", glm::value_ptr(tintColor));
		ImGui::Columns(1);
	}

	void PropertiesPanel::DrawCameraComponent(CameraComponent& cameraComponent)
	{
		ImGui::NewLine();
		ImGui::Text("Camera properties");

		ImGui::Columns(2);
		float columnWidth = 100.0f;
		ImGui::SetColumnWidth(0, columnWidth);

		auto& camera = cameraComponent.Camera;

		ImGui::Text("Primary");

		ImGui::NextColumn();
		ImGui::Checkbox("##Primary", &cameraComponent.Primary);
		ImGui::Columns(1);

		ImGui::Columns(2);

		ImGui::Text("View Size");

		ImGui::NextColumn();

		float orthoSize = camera.GetOrthographicSize();
		if (ImGui::DragFloat("##View Size", &orthoSize))
			camera.SetOrthographicSize(orthoSize);
		ImGui::Columns(1);

		ImGui::Columns(2);
		ImGui::Text("Near Clip");

		ImGui::NextColumn();
		float orthoNear = camera.GetOrthographicNearClip();
		if (ImGui::DragFloat("##Near Clip", &orthoNear))
			camera.SetOrthographicNearClip(orthoNear);
		ImGui::Columns(1);

		ImGui::Columns(2);
		ImGui::Text("Far Clip");

		ImGui::NextColumn();
		float orthoFar = camera.GetOrthographicFarClip();
		if (ImGui::DragFloat("##Far Clip", &orthoFar))
			camera.SetOrthographicFarClip(orthoFar);

		ImGui::Columns(1);
	}

}
