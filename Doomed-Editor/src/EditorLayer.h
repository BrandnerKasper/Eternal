#pragma once

#include <Eternal.h>

namespace Eternal {
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Eternal::Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Eternal::Event& event) override;
	private:
		OrthographicCameraController m_CameraController;

		//Temporary!!
		Ref<FrameBuffer> m_FrameBuffer;
		Ref<VertexArray> m_SquareVertexArray;
		Ref<Shader> m_FlatColorShader;
		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
		Ref<Texture2D> m_CheckerboardTexture;
		glm::vec3 m_Position = { -1.0f, 0.0f, 0.0f };
		glm::vec2 m_Scale = { 0.8f, 0.8f };
		int m_Tiling = 2;
		float m_Rotation = 30.0f;
		glm::vec4 m_TintColor = { 1.0f, 1.0f, 0.8f, 1.0f };

	};
}