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
		//This fcts should be in some kind of global UI libary!
		void DrawVecControl(const std::string& label, float& value, float resetValue, float columnWidth);
		void DrawVec2Control(const std::string& label, glm::vec2& values, float resetValue, float columnWidth);
		void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth);

		void DrawComponents(Entity entity);
		void DrawTagComponent(TagComponent& tagComponent);
		void DrawTransformComponent(TransformComponent& transformComponent);
		void DrawSpriteRenderComponent(SpriteRendererComponent& spriteRenderComponent);
		void DrawCameraComponent(CameraComponent& cameraComponent);
	private:
		Ref<SceneHierarchyPanel> m_SceneHierachyPanel;

	};
}