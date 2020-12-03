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

		HandlePlay();

		UpdatePhysics(ts);
		
		UpdateCameraRender(ts);

		UpdateScripts(ts);
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
		//Update Transform only when Transform changed
		{
			auto view = m_Registry.view<TransformComponent>();
			for (auto entity : view)
			{
				view.get<TransformComponent>(entity).CheckTransform();
			}
		}
	}

	void Scene::UpdateScripts(Timestep ts)
	{
		// Only Update Scripts when Button is pressed! TODO add play button!!
		if (m_play)
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
					if (nsc.ScriptReference)
					{
						// TODO: Move to Scene::OnScenePlay
						if (!nsc.Instance)
						{
							nsc.Instance = nsc.ScriptReference;
							//nsc.Instance = nsc.InstantiateScript();
							nsc.Instance->m_Entity = Entity{ entity, this };
							nsc.Instance->OnCreate();
						}

						nsc.Instance->OnUpdate(ts);
					}
				});
		}
	}

	void Scene::UpdateCameraRender(Timestep ts)
	{
		if (!m_play)
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

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		SortEntitysByZValue();

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

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			SortEntitysByZValue();

			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transform.Transform, sprite.Texture, sprite.TextureScale, sprite.Color);
			}
			Renderer2D::EndScene();
		}
	}

	void Scene::UpdatePhysics(Timestep ts)
	{
		if (m_play)
		{
			//Create all physics objects
			{
				auto view = m_Registry.view<TransformComponent, PhysicsComponent>();
				for (auto entity : view)
				{
					auto [transform, physics] = view.get<TransformComponent, PhysicsComponent>(entity);
					if (!physics.body)
					{
						physics.SetPhysics(b2Vec2(transform.Position.x, transform.Position.y), b2Vec2(transform.Size.x, transform.Size.y), transform.Rotation);
						physics.body = physicsWorld->CreateBody(&physics.bodyDef);
						physics.Fixture = physics.body->CreateFixture(&physics.fixtureDef);
						//physics.ApplyForce();
					}
					//physics.ApplyForce();
				}
			}

			//UpdateScripts(ts);
			//Start Physic Simulation
			physicsWorld->Update(ts);

			//Rewrite Position into optical position again!
			{
				auto view = m_Registry.view<TransformComponent, PhysicsComponent>();
				for (auto entity : view)
				{
					auto [transform, physics] = view.get<TransformComponent, PhysicsComponent>(entity);
					transform.Position = glm::vec3(physics.body->GetPosition().x, physics.body->GetPosition().y, transform.Position.z);
					transform.Rotation = (float)(physics.body->GetAngle() * 180.0f /3.141592f);
				}
			}
		}
	}

	void Scene::HandlePlay()
	{
		//Set ResetTransform
		if (m_play)
		{
			if (!m_initializeResetTransform)
			{
				m_initializeResetTransform = true;
				auto view = m_Registry.view<TransformComponent, PhysicsComponent>();
				for (auto entity : view)
				{
					auto [transform, physics] = view.get<TransformComponent, PhysicsComponent>(entity);

					//Safe Reset Transform
					transform.ResetPosition = transform.Position;
					transform.ResetRotation = transform.Rotation;
				}
			}
		}
		else
		{
			if (m_initializeResetTransform)
			{
				m_initializeResetTransform = false;
				auto view = m_Registry.view<TransformComponent, PhysicsComponent>();
				for (auto entity : view)
				{
					auto [transform, physics] = view.get<TransformComponent, PhysicsComponent>(entity);

					//Reset Transform when play is over
					transform.Position = transform.ResetPosition;
					transform.Rotation = transform.ResetRotation;

					//Reset Physics
					physics.body->DestroyFixture(physics.Fixture);
					physics.body = nullptr;
				}
			}
		}
	}

	void Scene::SortEntitysByZValue()//Fix Blending Problems with Batch Renderer by sorting all rendered entitys by z-position
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		group.sort<TransformComponent>([&group](const TransformComponent e1, const TransformComponent e2)
			{
				return e1.Position.z < e2.Position.z;
			});
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

	template<>
	void Scene::OnComponentAdded<PhysicsComponent>(Entity entity, PhysicsComponent& component)
	{
	}
}