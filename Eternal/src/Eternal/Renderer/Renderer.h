#pragma once

#include "RenderCommand.h"

#include "Camera.h"

#include "Shader.h"

namespace Eternal {

	class Renderer
	{
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene();
		static void EndScene();

		static void Submit(const SharedPtr<Shader>& shader,
							const SharedPtr<VertexArray>& vertexArray,
							const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		inline static float GetTime() { return RendererAPI::GetTime(); };

	private:
		struct SceneData
		{
			glm::mat4 ViewProjecionMatrix;
		};

		static SceneData* m_SceneData;
	};

}