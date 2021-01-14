#pragma once

#pragma warning(push, 0)
#include <entt.hpp>
#pragma warning(pop)

#include "Eternal/Core/Timestep.h"
#include "Eternal/Renderer/EditorCameraController.h"
#include "Eternal/Scene/Group.h"
#include "Eternal/Physics/PhysicsWorld.h"
#include "Eternal/Events/KeyEvent.h"
#include "Eternal/Audio/AudioManager.h"

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
		Entity GetEntityByTag(std::string tag);

		void CreateGroup(std::string name);
		void DestroyGroup(Group& group);

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);
		std::string GetName() { return m_Name; }
		void SetName(std::string name) { m_Name = name; }
		void SetPlay(bool play) { m_play = play; }
		bool GetPlay() { return m_play; }
		void OnEvent(Event& event);
		void PlayAudioFile(const Hazel::AudioSource& source);

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
		void InitAudio();
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
		
	private:
		entt::registry m_Registry;
		std::string m_Name = "Untitled";
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		Ref<EditorCameraController> m_EditorCamera;

		//Scene Hierachy
		std::vector<Group> m_Groups;

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