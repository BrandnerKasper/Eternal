#pragma once

#include "RenderCommand.h"

#include "Camera.h"

#include "Shader.h"

namespace Eternal {

	class Renderer
	{
	public:
		static void Init();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader,
							const Ref<VertexArray>& vertexArray,
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