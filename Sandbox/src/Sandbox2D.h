#pragma once

#include <Eternal.h>

class Sandbox2D : public Eternal::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Eternal::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Eternal::Event& event) override;
private:
	Eternal::EditorCameraController m_CameraController;

	//Temporary!!
	Eternal::Ref<Eternal::VertexArray> m_SquareVertexArray;
	Eternal::Ref<Eternal::Shader> m_FlatColorShader;
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	Eternal::Ref<Eternal::Texture2D> m_CheckerboardTexture;
	glm::vec3 m_Position = { -1.0f, 0.0f, 0.0f };
	glm::vec2 m_Scale = { 0.8f, 0.8f };
	int m_Tiling = 2;
	float m_Rotation = 30.0f;
	glm::vec4 m_TintColor = { 1.0f, 1.0f, 0.8f, 1.0f };

};