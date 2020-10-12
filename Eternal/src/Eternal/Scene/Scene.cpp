#include "ETpch.h"

#include "Scene.h"
#include "Component.h"
#include "Eternal/Renderer/Renderer2D.h"
#include "Entity.h"

#include <glm/glm.hpp>

namespace Eternal {

	Scene::Scene()
	{
		entt::entity entity = m_Registry.create();

		m_Registry.emplace<TransformComponent>(entity);
	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };

		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		entity.AddComponent<TransformComponent>();
		
		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}
}