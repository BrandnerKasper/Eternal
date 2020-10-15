#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "Eternal/Renderer/Texture.h"
#include "Eternal/Scene/SceneCamera.h"

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
		glm::mat4 Transform { 1.0f };
		glm::vec3 Position { 0.0f, 0.0f, 0.0f };
		glm::vec2 Size { 1.0f, 1.0f };
		float Rotation = 0.0f;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) {}
		TransformComponent(const glm::vec3& position, const glm::vec2& size, float rotation)
			: Position(position), Size(size), Rotation(rotation)
		{
			Transform = glm::translate(Transform, Position) 
				* glm::rotate(Transform, glm::radians(Rotation), { 0.0f, 0.0f, 1.0f })
				* glm::scale(Transform, { Size.x, Size.y, 1.0f });
		}
		TransformComponent(const glm::vec3& position)
			: Position(position)
		{
			Transform = glm::translate(Transform, Position)
				* glm::rotate(Transform, glm::radians(Rotation), { 0.0f, 0.0f, 1.0f })
				* glm::scale(Transform, { Size.x, Size.y, 1.0f });
		}
		TransformComponent(const glm::vec3& position, const glm::vec2& size)
			: Position(position), Size(size) 
		{
			Transform = glm::translate(Transform, Position)
				* glm::rotate(Transform, glm::radians(Rotation), { 0.0f, 0.0f, 1.0f })
				* glm::scale(Transform, { Size.x, Size.y, 1.0f });
		}

		//operator glm::mat4& () { return Transform; }
		//operator const glm::mat4& () const { return Transform; }
	};

	struct SpriteRendererComponent
	{
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
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

	};
}