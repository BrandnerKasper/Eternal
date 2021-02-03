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

	/// <summary>Handles the creation of an entity in the scene.</summary>
	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();

		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		
		return entity;
	}

	/// <summary>Handles the destruction of an entity in the scene.</summary>
	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	/**
	* Searches through all entitys to find the entity with the given name and return a copy of it.
	*
	* @param std::string The tag of the entity.
	* @return Entity a copy of the wanted entity.
	*/
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

	/// <summary>Handles the creation of groups in the scene.</summary>
	void Scene::CreateGroup(std::string name)
	{
		auto group = Group("Nameless Group" + std::to_string(Group::g_ID + 1));
		m_Groups.push_back(group);
	}

	/// <summary>Handles the desctruction of a group in the scene.</summary>
	void Scene::DestroyGroup(Group& group)
	{
		m_Groups.erase(std::remove(m_Groups.begin(), m_Groups.end(), group), m_Groups.end());
	}

	/// <summary>Handles the update loop of the scene.</summary>
	void Scene::OnUpdate(Timestep ts)
	{
		UpdateNonPhysicalTransforms();

		HandlePlay();

		UpdatePhysics(ts);
		
		UpdateCameraRender(ts);

		UpdateScripts(ts);
	}

	/**
	* Resizes editor and scene cameras when user resized the viewport panel.
	*
	* @param width and height of the viewport.
	* @return
	*/
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

	/// <summary>Updates transform of every object while in edit mode.</summary>
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

	/// <summary>Updates the scripts of each entity, but only when when in play mode.</summary>
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

	/// <summary>Decides which camera to render to. When in edit mode use editor camera, when in play mode use scene camera.</summary>
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

	/**
	* Updates editor camera input and use it to render.
	*
	* @param Timestep the frametime.
	* @return
	*/
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

	/**
	* Searches through all entitys to find the primary scene camera and then use it to render.
	*
	* @param Timestep the frametime.
	* @return
	*/
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

	/// <summary>Updates physics simulation!</summary>
	void Scene::UpdatePhysics(Timestep ts)
	{
		if (m_play)
		{
			InitPhysics();

			physicsWorld->Update(ts);

			UpdatePhysicalTransform();
		}
	}

	/// <summary>Intial all physics objects!</summary>
	void Scene::InitPhysics()
	{
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

	/// <summary>Rewrites the pysical position into optical position!</summary>
	void Scene::UpdatePhysicalTransform()
	{
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

	/// <summary>Handles the play state of the scene. Pressing the play button once saves a reset state, that when pressing the play button (now the stop button) again resets to the saved state.</summary>
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

	/// <summary>Sorts entitys by their Z value of their transformcomponent. Fixes blending problems with batch renderer!</summary>
	void Scene::SortEntitysByZValue()
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		group.sort<TransformComponent>([&group](const TransformComponent e1, const TransformComponent e2)
			{
				return e1.Position.z < e2.Position.z;
			});
	}

	/// <summary>Safes the transform of every gameobject with physics component</summary>
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

	/// <summary>Resets phxsics when the game is over</summary>
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

	/// <summary>Resets the script when the game is over</summary>
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

	/// <summary>Initials the Audio Manager</summary>
	void Scene::InitAudio()
	{
		AudioManager::Init();
	}

	/**
	* Cascades events down into every script of the game objects.
	*
	* @param Event& especially key events.
	* @return
	*/
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

	/**
	* Plays an audio file.
	*
	* @param &source needs to be a valid .mp3 file.
	* @return
	*/
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

	/**
	* When a camera component gets added to an entity, the camera viewport needs to be resized to the current Viewport
	*
	* @param Entity, CameraComponent& 
	* @return
	*/
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