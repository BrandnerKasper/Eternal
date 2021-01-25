#include "ETpch.h"

#include "Scene.h"
#include "Component.h"
#include "Eternal/Renderer/Renderer2D.h"
#include "Entity.h"

#include "Eternal/NativeScripts/Player.h"

#include <glm/glm.hpp>

namespace Eternal {

	Scene::Scene()
	{
		InitAudio();
	}

	Scene::Scene(std::string name)
	{
		InitAudio();
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

	Entity Scene::GetEntityByTag(std::string tag)
	{
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			auto& Tag = view.get<TagComponent>(entity).Tag;
			if (Tag._Equal(tag))
				return Entity{ entity, this };

		}
		ET_CORE_ASSERT(false, "No Entity found with this Tag!");
	}

	void Scene::CreateGroup(std::string name)
	{
		auto group = Group("Nameless Group" + std::to_string(Group::g_ID + 1));
		m_Groups.push_back(group);
	}

	void Scene::DestroyGroup(Group& group)
	{
		//ET_CORE_INFO("Destroy group with id {0}", group.m_ID);
		m_Groups.erase(std::remove(m_Groups.begin(), m_Groups.end(), group), m_Groups.end());
	}

	void Scene::OnUpdate(Timestep ts)
	{
		UpdateNonPhysicalTransforms();

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

	void Scene::UpdateNonPhysicalTransforms()
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
		// Only Update Scripts when Button is pressed!
		if (m_play)
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto& entity, auto& nsc)
				{
					if (nsc.ScriptReference)
					{
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
			if(m_EditorCamera)
				UpdateEditorCameraRender(ts);
		}
		else
		{
			UpdateSceneCameraRender(ts);
		}
	}

	void Scene::UpdateEditorCameraRender(Timestep ts)
	{
		//Update input for Camera only when Scene is focused
		if(m_SceneFocused)
			m_EditorCamera->OnUpdate(ts);
		//Use Editor Camera as Default!
		Renderer2D::BeginScene(m_EditorCamera->GetCamera(), m_EditorCamera->GetCamera().GetViewMatrix());

		SortEntitysByZValue();
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

			SortEntitysByZValue();
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

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
			InitPhysics();

			//Start Physic Simulation
			physicsWorld->Update(ts);

			UpdatePhysicalTransform();
		}
	}

	void Scene::InitPhysics()
	{
		//Create all physics objects
		{
			auto view = m_Registry.view<TransformComponent, PhysicsComponent>();
			for (auto entity : view)
			{
				auto [transform, physics] = view.get<TransformComponent, PhysicsComponent>(entity);
				if (!physics.body)
				{
					physics.SetPhysicProperties(b2Vec2(transform.Position.x, transform.Position.y), b2Vec2(transform.Size.x, transform.Size.y), transform.Rotation);
					physics.body = physicsWorld->CreateBody(&physics.bodyDef);
					physics.Fixture = physics.body->CreateFixture(&physics.fixtureDef);
				}
			}
		}
	}

	void Scene::UpdatePhysicalTransform()
	{
		//Rewrite Position into optical position again!
		{
			auto view = m_Registry.view<TransformComponent, PhysicsComponent>();
			for (auto entity : view)
			{
				auto [transform, physics] = view.get<TransformComponent, PhysicsComponent>(entity);
				transform.Position = glm::vec3(physics.body->GetPosition().x, physics.body->GetPosition().y, transform.Position.z);
				transform.Rotation = (float)(physics.body->GetAngle() * 180.0f / 3.141592f);
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
				SafeResetTransform();
			}
		}
		else
		{
			if (m_initializeResetTransform)
			{
				m_initializeResetTransform = false;

				ResetPhysics();

				ResetScripts();
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

	void Scene::SafeResetTransform()
	{
		auto view = m_Registry.view<TransformComponent, PhysicsComponent>();
		for (auto entity : view)
		{
			auto [transform, physics] = view.get<TransformComponent, PhysicsComponent>(entity);

			//Safe Reset Transform
			transform.ResetPosition = transform.Position;
			transform.ResetRotation = transform.Rotation;
		}
	}

	void Scene::ResetPhysics()
	{
		auto view = m_Registry.view<TransformComponent, PhysicsComponent>();
		for (auto entity : view)
		{
			auto [transform, physics] = view.get<TransformComponent, PhysicsComponent>(entity);

			//Reset Transform when play is over
			transform.Position = transform.ResetPosition;
			transform.Rotation = transform.ResetRotation;

			//Reset Physics
			physics.body->DestroyFixture(physics.Fixture);
			physicsWorld->DestroyBody(physics.body);
			physics.body = nullptr;
		}
	}

	void Scene::ResetScripts()
	{
		m_Registry.view<NativeScriptComponent>().each([=](auto& entity, auto& nsc)
			{
				if (nsc.ScriptReference)
				{
					if (nsc.Instance)
					{
						nsc.Instance->OnDestroy();
						nsc.Instance = nullptr;
					}
				}
			});
	}

	void Scene::InitAudio()
	{
		AudioManager::Init();
	}

	void Scene::OnEvent(Event& event)
	{
		//Scene Entity Scripts Events
		if (m_play)
		{
			auto view = m_Registry.view<NativeScriptComponent>();
			for each (auto entity in view)
			{
				auto nsc = view.get<NativeScriptComponent>(entity);

				if (nsc.ScriptReference)
				{
					if (nsc.Instance)
					{
						nsc.Instance->OnEvent(event);
					}
				}
			}
		}
	}

	void Scene::PlayAudioFile(const Hazel::AudioSource& source)
	{
		AudioManager::Play(source);
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

	template<>
	void Scene::OnComponentAdded<AudioComponent>(Entity entity, AudioComponent& component)
	{
	}
}