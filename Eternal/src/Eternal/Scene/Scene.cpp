#include "ETpch.h"

#include "Scene.h"
#include "Component.h"
#include "Eternal/Renderer/Renderer2D.h"
#include "Entity.h"

#include <glm/glm.hpp>

namespace Eternal {

	Scene::Scene()
	{
		
	}

	Scene::Scene(std::string name)
	{
		m_Name = name;
	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();

		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(Timestep ts)
	{
		UpdateTransforms();

		UpdateScripts(ts, true);
		
		UpdateCameraRender(ts, false);
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		//Resize EditorCamera
		m_EditorCamera->OnViewportResize(width, height);

		//Resize non fixed Aspect ratio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.Camera.SetViewportSize(width, height);
			}

		}
	}

	void Scene::UpdateTransforms()
	{
		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view)
		{
			view.get<TransformComponent>(entity).CheckTransform();
		}
	}

	void Scene::UpdateScripts(Timestep ts, bool play)
	{
		// Only Update Scripts when Button is pressed! TODO add play button!!
		if (play)
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
					// TODO: Move to Scene::OnScenePlay
					if (!nsc.Instance)
					{
						nsc.Instance = nsc.InstantiateScript();
						nsc.Instance->m_Entity = Entity{ entity, this };
						nsc.Instance->OnCreate();
					}

					nsc.Instance->OnUpdate(ts);
				});
		}
	}

	void Scene::UpdateCameraRender(Timestep ts, bool play)
	{
		if (!play)
		{
			UpdateEditorCameraRender(ts);
		}
		else
		{
			UpdateSceneCameraRender(ts);
		}
	}

	void Scene::UpdateEditorCameraRender(Timestep ts)
	{
		//Use Editor Camera as Default!
		Renderer2D::BeginScene(m_EditorCamera->GetCamera());

		//auto view = m_Registry.view<TransformComponent,SpriteRendererComponent>();
		//Sort this group by entity transform position z value but how...
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::DrawQuad(transform.Transform, sprite.Texture, sprite.TextureScale, sprite.Color);
		}
		Renderer2D::EndScene();
	}

	void Scene::UpdateSceneCameraRender(Timestep ts)
	{
		//When Scene plays use Scene Camera (the primary one!)
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;

		auto view = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view)
		{
			auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

			if (camera.Primary)
			{
				mainCamera = &camera.Camera;
				cameraTransform = transform.Transform;
				break;
			}
		}

		if (mainCamera)
		{
			//Render Scene
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			//auto view = m_Registry.view<TransformComponent,SpriteRendererComponent>();
			//Sort this group by entity transform position z value but how...
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transform.Transform, sprite.Texture, sprite.TextureScale, sprite.Color);
			}
			Renderer2D::EndScene();
		}
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}
}