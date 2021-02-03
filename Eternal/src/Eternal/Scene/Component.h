#pragma once

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <box2d/box2d.h>
#include <HazelAudio.h>
#pragma warning(pop)

#include "Eternal/Renderer/Texture.h"
#include "Eternal/Scene/SceneCamera.h"
#include "Eternal/Scene/ScriptableEntity.h"
#include "Eternal/Renderer/Texture.h"

namespace Eternal {

	struct TagComponent
	{
		std::string Tag;
		int Group_ID = 0;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::mat4 stdMat{ 1.0f };
		glm::mat4 Transform { 1.0f };
		glm::vec3 Position { 0.0f, 0.0f, 0.0f };
		glm::vec2 Size { 1.0f, 1.0f };
		float Rotation = 0.0f;

		//Check variables for CeckTransform to only calculate Transform if necessary
		glm::vec3 CheckPosition { 0.0f, 0.0f, 0.0f };
		glm::vec2 CheckSize { 1.0f, 1.0f };
		float CheckRotation = 0.0f;

		//ResetPhysics variables to reset status before the physic simulation
		glm::vec3 ResetPosition {1.0f};
		float ResetRotation = 0.0f;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) {}
		
		TransformComponent(const glm::vec3& position)
			: Position(position)
		{
			CalculateTransform();
		}
		TransformComponent(const glm::vec3& position, const glm::vec2& size)
			: Position(position), Size(size) 
		{
			CalculateTransform();
		}
		TransformComponent(const glm::vec3& position, const glm::vec2& size, float rotation)
			: Position(position), Size(size), Rotation(rotation)
		{
			CalculateTransform();
		}

		void CheckTransform()
		{
			if (TransformChanged())
				CalculateTransform();
		}

		void CalculateTransform()
		{
			Transform = glm::translate(stdMat, Position)
				* glm::rotate(stdMat, glm::radians(Rotation), { 0.0f, 0.0f, 1.0f })
				* glm::scale(stdMat, { Size.x, Size.y, 1.0f });
		}

		bool TransformChanged()
		{
			if (CheckPosition != Position || CheckSize != Size || CheckRotation != Rotation)
			{
				CheckPosition = Position;
				CheckSize = Size;
				CheckRotation = Rotation;
				return true;
			}
			else
				return false;
		}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct SpriteRendererComponent
	{
		std::string TextureFilepath = "";
		Ref<Texture2D> Texture = nullptr;
		int TextureScale = 1;

		glm::vec4 Color {1.0f, 1.0f, 1.0f, 1.0f};

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(glm::vec4 color)
			: Color(color) {}
		SpriteRendererComponent(const Ref<Texture2D>& texture, const int textureScale, glm::vec4 tintColor)
			: Texture(texture), TextureScale(textureScale), Color(tintColor) {}
		SpriteRendererComponent(const Ref<Texture2D>& texture)
			: Texture(texture){}
		SpriteRendererComponent(const std::string textureFilepath)
			: TextureFilepath(textureFilepath) 
		{
			Texture = Texture2D::Create(TextureFilepath);
		}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = false;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		bool GetPrimary() { return Primary; }
		void SetPrimary(bool primary) { Primary = primary; }
	};

	struct NativeScriptComponent
	{
		std::string ScriptName = "";
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* ScriptReference = nullptr;
		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		void SetScript(ScriptableEntity* script)
		{
			delete Instance;
			Instance = nullptr;
			ScriptReference = script;
			//DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}

		//Lets leave it for now...
		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}

		NativeScriptComponent() = default;
	};

	struct PhysicsComponent
	{
		enum class BodyShape
		{
			box,
			circle
		};

		BodyShape bodyShape = BodyShape::box;
		b2BodyDef bodyDef;
		b2BodyType bodyType = b2_staticBody;
		b2CircleShape circleShape;
		b2PolygonShape polygonBodyShape; 
		b2FixtureDef fixtureDef; //density in kg/m2, friction [0,1], restitution [0,1], isSensor
		b2Fixture* Fixture; 
		b2Body* body = nullptr;

		void SetPhysicProperties(b2Vec2 position, b2Vec2 size, float angle)
		{
			bodyDef.position.Set(position.x, position.y);
			bodyDef.angle = glm::radians(angle);

			switch (bodyShape)
			{
			case BodyShape::box:
				polygonBodyShape.SetAsBox(size.x / 2, size.y / 2);
				fixtureDef.shape = &polygonBodyShape;
				break;
			case BodyShape::circle:
				if(size.x != size.y)
					ET_CORE_ERROR("Circles need to be round, so width and height need to be the same :)!");
				circleShape.m_radius = size.x / 2;
				fixtureDef.shape = &circleShape;
				break;
			default:
				break;
			}

			switch (bodyType)
			{
			case b2BodyType::b2_staticBody:
				bodyDef.type = b2_staticBody;
				fixtureDef.density = 0.0f;
				break;
			case b2BodyType::b2_kinematicBody:
				bodyDef.type = b2_kinematicBody;
				fixtureDef.density = 0.0f;
				break;
			case b2BodyType::b2_dynamicBody:
				bodyDef.type = b2_dynamicBody;
				if (fixtureDef.density == 0.0f)
					fixtureDef.density = 1.0f;
				break;
			default:
				break;
			}
		}

		PhysicsComponent() = default;
	};

	struct AudioComponent
	{
		std::string filename = "";
		std::string AudioFilePath = "";
		Hazel::AudioSource file = Hazel::AudioSource::LoadFromFile(AudioFilePath);
		bool Looping = false;
		float Gain = 1.0f;
		float Pitch = 1.0f;

		void LoadFile()
		{
			file = Hazel::AudioSource::LoadFromFile(AudioFilePath);
		}

		void SetLoop()
		{
			file.SetLoop(Looping);
		}

		void SetGain()
		{
			file.SetGain(Gain);
		}

		void SetPitch()
		{
			file.SetPitch(Pitch);
		}

		AudioComponent() = default;
	};
}