#include "PropertiesPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

#include "Eternal/Utils/PlatformUtils.h"
#include "Eternal/NativeScripts/ScriptHandler.h"
#include "Eternal/Scene/Component.h"

//TODO: TOOLTIPS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

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
		//Draw Entity properties
		auto& entity = m_SceneHierachyPanel->m_SelectedEntity;
		if (entity)
		{
			DrawComponents(entity);

			HandleAddComponentButton(entity);
		}

		//Draw Group properties
		auto selectedGroup = m_SceneHierachyPanel->m_SelectedGroup;
		if (selectedGroup)
		{
			DrawGroup(selectedGroup);
		}
		ImGui::End();
	}

	static void DrawVecControl(const std::string& label, float& value, float resetValue = 0.0f, float columnWidth = 100.0f)
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
		ImGui::DragFloat("##R", &value, 1.0f, 0.0f, 360.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	static void DrawVec2Control(const std::string& label, glm::vec2& values, float resetValue = 1.0f, float columnWidth = 100.0f)
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

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
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

	template<typename T, typename UIFunction>
	static void PropertiesPanel::HandleComponent(const std::string& name, Entity entity, UIFunction uiFunction, bool showRemove)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			if (showRemove)
			{
				ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
				if (ImGui::Button("-", ImVec2{ lineHeight, lineHeight }))
				{
					ImGui::OpenPopup("ComponentSettings");
				}
			}
			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void PropertiesPanel::DrawComponents(Entity entity)
	{
		HandleComponent<TagComponent>("Tag", entity, [](auto& component)
			{
				DrawTagComponent(component);
			}, false);

		HandleComponent<TransformComponent>("Transform", entity, [](auto& component)
			{
				DrawTransformComponent(component);
			});

		HandleComponent<SpriteRendererComponent>("Sprite Properties", entity, [](auto& component)
			{
				DrawSpriteRenderComponent(component);
			});

		HandleComponent<CameraComponent>("Camera Properties", entity, [](auto& component)
			{
				DrawCameraComponent(component);
			});

		HandleComponent<NativeScriptComponent>("Native Script Properties", entity, [](auto& component)
			{
				DrawNativeScriptComponent(component);
			});

		HandleComponent<PhysicsComponent>("Physics Properties", entity, [](auto& component)
			{
				DrawPhysicsComponent(component);
			});
	}

	void PropertiesPanel::DrawGroup(Group* group)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();
		bool open = ImGui::TreeNodeEx("Group", treeNodeFlags);
		ImGui::PopStyleVar();

		if (open)
		{

			float columnWidth = 100.0f;

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, columnWidth);

			auto& tag = group->m_Name;
			ImGui::Text("Name");

			ImGui::NextColumn();
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("##TagGroup", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}

			ImGui::Columns(1);

			ImGui::TreePop();
		}
	}

	static void DrawTagComponent(TagComponent& tagComponent)
	{
		float columnWidth = 100.0f;
		
		ImGui::Columns(2);
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

	static void DrawTransformComponent(TransformComponent& transformComponent)
	{
		//z-Position should be between near and far clip of camera!
		auto& position = transformComponent.Position;
		DrawVec3Control("Position", position);

		auto& size = transformComponent.Size;
		DrawVec2Control("Scale", size);

		auto& rotation = transformComponent.Rotation;
		DrawVecControl("Rotation", rotation);
	}

	static void DrawSpriteRenderComponent(SpriteRendererComponent& spriteRenderComponent)
	{
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

		//Change Texture Button!
		//Opens Filemanager and lets you load a Texture!
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight * 4, lineHeight };
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.79f, 0.63f, 0.0f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.89f, 0.73f, 0.1f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.89f, 0.73f, 0.1f, 1.0f });
		ImGui::PushFont(boldFont);

		if (ImGui::Button("Texture", buttonSize))
		{
			std::string filepath = FileDialogs::OpenFile("Textures (*.png)\0*.png\0");
			if (!filepath.empty())
			{
				spriteRenderComponent.TextureFilepath = filepath;
				spriteRenderComponent.Texture = Texture2D::Create(spriteRenderComponent.TextureFilepath);
			}
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::Columns(2);
		float columnWidth = 100.0f;
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text("Tint Color");

		ImGui::NextColumn();
		auto& tintColor = spriteRenderComponent.Color;
		ImGui::ColorEdit4("##Tint Color", glm::value_ptr(tintColor));
		ImGui::Columns(1);
	}

	static void DrawCameraComponent(CameraComponent& cameraComponent)
	{
		float columnWidth = 100.0f;
		auto& camera = cameraComponent.Camera;

		//Camera Type List
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text("Camera Perspective Type");
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("I am a tooltip over Camera Perspective Type!");

		ImGui::NextColumn();
		
		int selected_cameraType = (int)camera.GetProjectionType();
		const char* cameraTypeNames[] = { "Perspective", "Orthographic" };

		// Simple selection popup (if you want to show the current selection inside the Button itself,
		// you may want to build a string using the "###" operator to preserve a constant ID with a variable label)
		if (ImGui::Button("Select."))
			ImGui::OpenPopup("Camera Type PopUp");
		ImGui::SameLine();
		ImGui::TextUnformatted(selected_cameraType == -1 ? "<None>" : cameraTypeNames[selected_cameraType]);
		if (ImGui::BeginPopup("Camera Type PopUp"))
		{
			for (int i = 0; i < IM_ARRAYSIZE(cameraTypeNames); i++)
				if (ImGui::Selectable(cameraTypeNames[i]))
				{
					selected_cameraType = i;
					camera.SetProjectionType((SceneCamera::ProjectionType)selected_cameraType);
				}
			ImGui::EndPopup();
		}
		ImGui::Columns(1);

		//Primary Checkbox
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);

		ImGui::Text("Primary");

		ImGui::NextColumn();
		ImGui::Checkbox("##Primary", &cameraComponent.Primary); //Handle multiple Cameras and set other Camera Primarys to false
		ImGui::Columns(1);


		//Perspective
		if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
		{
			ImGui::Columns(2);

			ImGui::Text("Field Of View");

			ImGui::NextColumn();

			float perspectiveVerticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
			if (ImGui::DragFloat("##Field Of View", &perspectiveVerticalFov))
				camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));
			ImGui::Columns(1);

			ImGui::Columns(2);
			ImGui::Text("Near Clip");

			ImGui::NextColumn();
			float perspectiveNear = camera.GetPerspectiveNearClip();
			if (ImGui::DragFloat("##Near Clip", &perspectiveNear))
				camera.SetPerspectiveNearClip(perspectiveNear);
			ImGui::Columns(1);

			ImGui::Columns(2);
			ImGui::Text("Far Clip");

			ImGui::NextColumn();
			float perspectiveFar = camera.GetPerspectiveFarClip();
			if (ImGui::DragFloat("##Far Clip", &perspectiveFar))
				camera.SetPerspectiveFarClip(perspectiveFar);

			ImGui::Columns(1);
		}

		//Orthographic
		if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
		{
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

	static void DrawNativeScriptComponent(NativeScriptComponent& nsc)
	{
		if (!nsc.ScriptName.empty())
		{
			ImGui::Columns(2);
			float columnWidth = 100.0f;
			ImGui::SetColumnWidth(0, columnWidth);
			ImGui::Text("Script");

			ImGui::NextColumn();

			ImGui::Text(nsc.ScriptName.c_str());
			ImGui::Columns(1);
		}
		//Change Script Button!
		//Opens Filemanager and lets you load a Script!
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight * 4, lineHeight };
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.79f, 0.63f, 0.0f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.89f, 0.73f, 0.1f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.89f, 0.73f, 0.1f, 1.0f });
		ImGui::PushFont(boldFont);

		if (ImGui::Button("Script", buttonSize))
		{
			std::string filepath = FileDialogs::OpenFile("Scripts (*.h)\0*.h\0");
			if (!filepath.empty())
			{
				nsc.ScriptFilepath = filepath;
				nsc.ScriptName = FileDialogs::GetFileName(filepath);
				auto scriptNumber = ScriptHandler::GetScriptNumber(nsc.ScriptName);
				nsc.SetScript(ScriptHandler::GetScript(scriptNumber));
			}
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
	}

	static void DrawPhysicsComponent(PhysicsComponent& phc)
	{
		float columnWidth = 100.0f;

		//BodyShape PopUp List
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text("Body Shape");
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("I am a tooltip over Body Shape!");

		ImGui::NextColumn();
		auto& bodyShape = phc.bodyShape;
		int selected_bodyShape = (int)bodyShape;
		const char* bodyShapesNames[] = { "Box", "Circle" };

		// Simple selection popup (if you want to show the current selection inside the Button itself,
		// you may want to build a string using the "###" operator to preserve a constant ID with a variable label)
		if (ImGui::Button("Select.."))
			ImGui::OpenPopup("bodyShape PopUp");
		ImGui::SameLine();
		ImGui::TextUnformatted(selected_bodyShape == -1 ? "<None>" : bodyShapesNames[selected_bodyShape]);
		if (ImGui::BeginPopup("bodyShape PopUp"))
		{
			for (int i = 0; i < IM_ARRAYSIZE(bodyShapesNames); i++)
				if (ImGui::Selectable(bodyShapesNames[i]))
				{
					selected_bodyShape = i;
					bodyShape = (PhysicsComponent::BodyShape)selected_bodyShape;
				}
			ImGui::EndPopup();
		}
		ImGui::Columns(1);

		//BodyType PopUp List
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text("Body Type");
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("I am a tooltip over BodyType!");
		
		ImGui::NextColumn();
		auto& bodyType = phc.bodyType;
		int selected_bodyType = (int)bodyType;
		const char* bodyTypesnames[] = { "Static", "Kinematic", "Dynamic" };
		
		// Simple selection popup (if you want to show the current selection inside the Button itself,
		// you may want to build a string using the "###" operator to preserve a constant ID with a variable label)
		if (ImGui::Button("Select..."))
			ImGui::OpenPopup("bodyType PopUp");
		ImGui::SameLine();
		ImGui::TextUnformatted(selected_bodyType == -1 ? "<None>" : bodyTypesnames[selected_bodyType]);
		if (ImGui::BeginPopup("bodyType PopUp"))
		{
			for (int i = 0; i < IM_ARRAYSIZE(bodyTypesnames); i++)
				if (ImGui::Selectable(bodyTypesnames[i]))
				{
					selected_bodyType = i;
					bodyType = (b2BodyType)selected_bodyType;
				}
			ImGui::EndPopup();
		}
		ImGui::Columns(1);

		//Density Slider
		if (phc.bodyType == b2BodyType::b2_dynamicBody)
		{
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, columnWidth);
			ImGui::Text("Density");

			ImGui::NextColumn();
			auto& density = phc.fixtureDef.density;
			ImGui::SliderFloat("##Density", &density, 0.0f, 100.0f);
			ImGui::Columns(1);
		}

		//Friction Slider
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text("Friction");

		ImGui::NextColumn();
		auto& friction = phc.fixtureDef.friction;
		ImGui::SliderFloat("##Friction", &friction, 0.0f, 1.0f);
		ImGui::Columns(1);

		//Restitution Slider
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text("Restitution");

		ImGui::NextColumn();
		auto& restitution = phc.fixtureDef.restitution;
		ImGui::SliderFloat("##Restitution", &restitution, 0.0f, 1.0f);
		ImGui::Columns(1);
	}

	void PropertiesPanel::HandleAddComponentButton(Entity entity)
	{
		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (!entity.HasComponent<TransformComponent>())
			{
				if (ImGui::MenuItem("TransformComponent"))
				{
					entity.AddComponent<TransformComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!entity.HasComponent<SpriteRendererComponent>())
			{
				if (ImGui::MenuItem("SpriteRenderComponent"))
				{
					entity.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!entity.HasComponent<CameraComponent>())
			{
				if (ImGui::MenuItem("Camera"))
				{
					entity.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!entity.HasComponent<NativeScriptComponent>())
			{
				if (ImGui::MenuItem("Script"))
				{
					entity.AddComponent<NativeScriptComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!entity.HasComponent<PhysicsComponent>())
			{
				if (ImGui::MenuItem("Physics"))
				{
					entity.AddComponent<PhysicsComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}
	}
}
