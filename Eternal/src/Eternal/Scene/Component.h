#pragma once

#include <glm/glm.hpp>
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
		//Refactor to take in position rotation and scale!
		glm::mat4 Transform { 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct SpriteRendererComponent
	{
		Ref<Texture2D> Texture = Texture2D::Create(1, 1);
		int TextureScale = 1;

		glm::vec4 Color {1.0f, 1.0f, 1.0f, 1.0f};

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(glm::vec4 color)
			: Color(color) {}
		SpriteRendererComponent(const Ref<Texture2D>& texture, const int textureScale, glm::vec4 tintColor)
			: Texture(texture), TextureScale(textureScale), Color(tintColor) {}
	};
}