#pragma once

#include "entt.hpp"

#include "Eternal/Core/Timestep.h"

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
		std::string GetName() { return m_Name; }
		void SetName(std::string name) { m_Name = name; }
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		std::string m_Name = "Untitled";

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneViewportPanel;
		friend class SettingsPanel;
		friend class SceneSerializer;
	};
}