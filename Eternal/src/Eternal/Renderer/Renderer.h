#pragma once

#include "RenderCommand.h"

#include "Camera.h"

#include "Shader.h"

namespace Eternal {

	class Renderer
	{
	public:
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

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