#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Eternal/Renderer/Texture.h"
#include "Eternal/Scene/SceneCamera.h"
#include "Eternal/Scene/ScriptableEntity.h"
#include "Eternal/Renderer/Texture.h"

namespace Eternal {

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		//Refactor so that Maybe when Transform gets newly Calculated every time someting changes
		glm::mat4 stdMat{ 1.0f };
		glm::mat4 Transform { 1.0f };
		glm::vec3 Position { 0.0f, 0.0f, 0.0f };
		glm::vec2 Size { 1.0f, 1.0f };
		float Rotation = 0.0f;

		glm::vec3 OldPosition;
		glm::vec2 OldSize;
		float OldRotation;

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
			if (OldPosition != Position || OldSize != Size || OldRotation != Rotation)
			{
				OldPosition = Position;
				OldSize = Size;
				OldRotation = Rotation;
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
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

	};

	struct NativeScriptComponent
	{
		std::string ScriptFilepath = "";
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
}