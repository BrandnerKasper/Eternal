#include "PropertiesPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

#include "Eternal/Utils/PlatformUtils.h"
#include "Eternal/NativeScripts/ScriptHandler.h"
#include "Eternal/Scene/Component.h"

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

	static void DrawVecControl(const std::string& label, const std::string& tooltip, float& value, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(tooltip.c_str());

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
		ImGui::DragFloat("##R", &value, 1.0f, -360.0f, 360.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	static void DrawVec2Control(const std::string& label, const std::string& tooltip, glm::vec2& values, float resetValue = 1.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(tooltip.c_str());
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

	static void DrawVec3Control(const std::string& label, const std::string& tooltip, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(tooltip.c_str());
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

		HandleComponent<AudioComponent>("Audio Properties", entity, [](auto& component)
			{
				DrawAudioComponent(component);
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
		DrawVec3Control("Position", "Position in the 3D world.", position);

		auto& size = transformComponent.Size;
		DrawVec2Control("Scale", "Scales the object.", size);

		auto& rotation = transformComponent.Rotation;
		DrawVecControl("Rotation", "Rotate the object in the XY plane.", rotation);
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
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("UV Scaling determines how often the texture repeats itself.");


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
			std::optional<std::string> filepath = *FileDialogs::OpenFile("Textures (*.png)\0*.png\0");
			if (filepath)
			{
				auto filename = *FileDialogs::GetFileName(*filepath);
				spriteRenderComponent.TextureFilepath = "assets/textures/" + filename;
				spriteRenderComponent.Texture = Texture2D::Create(spriteRenderComponent.TextureFilepath);
			}
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::Columns(2);
		float columnWidth = 100.0f;
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text("Tint Color");
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Adds a tint over the given texture.");

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
			ImGui::SetTooltip("There a 2 types of cameras. Orthographic cameras display eyerything in a 2D plane, while Perspective cameras have a depth effect.");

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
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Set 1 of your Scene cameras as primary so the game knows which camera to use for rendering while the game is in playing state.");

		ImGui::NextColumn();
		ImGui::Checkbox("##Primary", &cameraComponent.Primary); //Handle multiple Cameras and set other Camera Primarys to false
		ImGui::Columns(1);


		//Perspective
		if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
		{
			ImGui::Columns(2);

			ImGui::Text("Field Of View");
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Field of View is the angle of the horizontal and vertical sight radius.");

			ImGui::NextColumn();

			float perspectiveVerticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
			if (ImGui::DragFloat("##Field Of View", &perspectiveVerticalFov))
				camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));
			ImGui::Columns(1);

			ImGui::Columns(2);
			ImGui::Text("Near Clip");
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Basically how near in front of the camera can it see things.");

			ImGui::NextColumn();
			float perspectiveNear = camera.GetPerspectiveNearClip();
			if (ImGui::DragFloat("##Near Clip", &perspectiveNear))
				camera.SetPerspectiveNearClip(perspectiveNear);
			ImGui::Columns(1);

			ImGui::Columns(2);
			ImGui::Text("Far Clip");
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Basically how far can the camera see things.");

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
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("How much can the camera see.");

			ImGui::NextColumn();

			float orthoSize = camera.GetOrthographicSize();
			if (ImGui::DragFloat("##View Size", &orthoSize))
				camera.SetOrthographicSize(orthoSize);
			ImGui::Columns(1);

			ImGui::Columns(2);
			ImGui::Text("Near Clip");
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("How near can the camera see. Orthographic cameras typically have a very slight latitude, since they project everything in 2D plane.");

			ImGui::NextColumn();
			float orthoNear = camera.GetOrthographicNearClip();
			if (ImGui::DragFloat("##Near Clip", &orthoNear))
				camera.SetOrthographicNearClip(orthoNear);
			ImGui::Columns(1);

			ImGui::Columns(2);
			ImGui::Text("Far Clip");
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("How far can the camera see.Orthographic cameras typically have a very slight latitude, since they project everything in 2D plane.");

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
			std::optional<std::string> filepath = FileDialogs::OpenFile("Scripts (*.h)\0*.h\0");
			if (filepath)
			{
				nsc.ScriptName = *FileDialogs::GetFileName(*filepath);
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
			ImGui::SetTooltip("The Body Shape determines the physical behaviour of the shape of the drawn object!");

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
			ImGui::SetTooltip("The Body Type determines the influence of physical forces on the body.\n Static means no forces have effect, Kinematic a linear velocity can be set (good for moving platforms)\n and Dynamic everything has effect, especially gravity!");
		
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
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("The Density determines the weight of the object, the heavier the harder to move!");

			ImGui::NextColumn();
			auto& density = phc.fixtureDef.density;
			ImGui::SliderFloat("##Density", &density, 0.0f, 100.0f);
			ImGui::Columns(1);
		}

		//Friction Slider
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text("Friction");
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("The Friction determines a force between two surfaces that are sliding, or trying to slide, across each other!");

		ImGui::NextColumn();
		auto& friction = phc.fixtureDef.friction;
		ImGui::SliderFloat("##Friction", &friction, 0.0f, 1.0f);
		ImGui::Columns(1);

		//Restitution Slider
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text("Restitution");
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("The Resitution determines how much energy remains after a collsion, high Restituion equals a flummi!");

		ImGui::NextColumn();
		auto& restitution = phc.fixtureDef.restitution;
		ImGui::SliderFloat("##Restitution", &restitution, 0.0f, 1.0f);
		ImGui::Columns(1);
	}

	static void DrawAudioComponent(AudioComponent& audioComponent)
	{
		if (!audioComponent.filename.empty())
		{
			ImGui::Columns(2);
			float columnWidth = 100.0f;
			ImGui::SetColumnWidth(0, columnWidth);
			ImGui::Text("Audio File");

			ImGui::NextColumn();

			ImGui::Text(audioComponent.filename.c_str());
			ImGui::Columns(1);

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, columnWidth);
			ImGui::Text("Gain");
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("For simplicity determines the volume");


			ImGui::NextColumn();

			ImGui::DragFloat("##Gain", &audioComponent.Gain, 0.1f, 0.0f, 1.0f, "%.1f");
			ImGui::Columns(1);

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, columnWidth);
			ImGui::Text("Pitch");
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("The Pitch determines the highness or lowness of sound!");


			ImGui::NextColumn();

			ImGui::DragFloat("##Pitch", &audioComponent.Pitch, 0.1f, 0.0f, 1.0f, "%.1f");
			ImGui::Columns(1);

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, columnWidth);
			ImGui::Text("Looping");
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("If the audio file finished, it simply plays again.");


			ImGui::NextColumn();

			ImGui::Checkbox("##Looping", &audioComponent.Looping); //Handle multiple Cameras and set other Camera Primarys to false
			ImGui::Columns(1);
		}
		//Change Audio Button!
		//Opens Filemanager and lets you load a AudioMP3File!
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight * 4, lineHeight };
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.79f, 0.63f, 0.0f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.89f, 0.73f, 0.1f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.89f, 0.73f, 0.1f, 1.0f });
		ImGui::PushFont(boldFont);

		if (ImGui::Button("Audio", buttonSize))
		{
			std::optional<std::string> filepath = FileDialogs::OpenFile("Audio (*.mp3)\0*.mp3\0");
			if (filepath)
			{
				audioComponent.filename = *FileDialogs::GetFileName(*filepath);
				audioComponent.AudioFilePath = "assets/audio/" + audioComponent.filename;
				audioComponent.LoadFile();
			}
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
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

			if (!entity.HasComponent<AudioComponent>())
			{
				if (ImGui::MenuItem("Audio"))
				{
					entity.AddComponent<AudioComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}
	}
}
