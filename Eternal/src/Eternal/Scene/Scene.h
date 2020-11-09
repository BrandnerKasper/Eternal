#pragma once

#include "entt.hpp"

#include "Eternal/Core/Timestep.h"

#include "Eternal/Renderer/EditorCameraController.h"

namespace Eternal
{
	class Entity;

	class Scene 
	{
	public:
		Scene();
		Scene(std::string name);
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);
		std::string GetName() { return m_Name; }
		void SetName(std::string name) { m_Name = name; }
	private:
		void UpdateTransforms();
		void UpdateScripts(Timestep ts, bool play);
		void UpdateCameras(Timestep ts, bool play);
		void UpdateEditorCamera(Timestep ts);
		void UpdateSceneCamera(Timestep ts);
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		std::string m_Name = "Untitled";
		Ref<EditorCameraController> m_EditorCamera;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneViewportPanel;
		friend class SettingsPanel;
		friend class SceneSerializer;
	};
}