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
	Eternal::OrthographicCameraController m_CameraController;

	//Temporary!!
	Eternal::Ref<Eternal::VertexArray> m_SquareVertexArray;
	Eternal::Ref<Eternal::Shader> m_FlatColorShader;
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	Eternal::Ref<Eternal::Texture2D> m_CheckerboardTexture;
	int m_Scale = 2;
	glm::vec4 m_TintColor = { 1.0f, 1.0f, 0.8f, 1.0f };

};