#pragma once

#include <entt.hpp>

#include "Eternal/Core/Timestep.h"

#include "Eternal/Renderer/EditorCameraController.h"

#include "Eternal/Physics/PhysicsWorld.h"

namespace Eternal
{
	struct Group
	{
		Group() = default;
		Group(std::string name) : m_Name(name), m_ID(++g_ID) {}
		Group(std::string name, int id) : m_Name(name), m_ID(id)
		{
			++g_ID;
		}
		std::string m_Name = "";
		int m_ID = 0;
		static inline uint32_t g_ID = 0;
	};

	class Entity;

	class Scene 
	{
	public:
		Scene();
		Scene(std::string name);
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);
		Entity GetEntityByTag(std::string tag);

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);
		std::string GetName() { return m_Name; }
		void SetName(std::string name) { m_Name = name; }
		void SetPlay(bool play) { m_play = play; }
		bool GetPlay() { return m_play; }

	private:
		void UpdateNonPhysicalTransforms();
		void UpdateScripts(Timestep ts);
		void UpdateCameraRender(Timestep ts);
		void UpdateEditorCameraRender(Timestep ts);
		void UpdateSceneCameraRender(Timestep ts);
		void UpdatePhysics(Timestep ts);
		void InitPhysics();
		void UpdatePhysicalTransform();
		void HandlePlay();
		void SortEntitysByZValue();
		void SafeResetTransform();
		void ResetPhysics();
		void ResetScripts();
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
		
	private:
		entt::registry m_Registry;
		std::string m_Name = "Untitled";
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		Ref<EditorCameraController> m_EditorCamera;

		//Scene Hierachy
		std::vector<Group*> m_Groups;

		//Play and reset conditions
		bool m_SceneFocused;
		bool m_play = false;
		bool m_initializeResetTransform = false;
		bool m_initializeResetPhysics = false;

		//Physics
		Ref<PhysicsWorld> physicsWorld = CreateRef<PhysicsWorld>();

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneViewportPanel;
		friend class SettingsPanel;
		friend class SceneSerializer;
	};
}