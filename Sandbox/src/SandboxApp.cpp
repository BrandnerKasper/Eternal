#include "ETpch.h"

#include <Eternal.h>

class ExampleLayer : public Eternal::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		ET_INFO("ExampleLayer::Update");
	}

	void OnEvent(Eternal::Event& event) override
	{
		ET_TRACE("{0}", event);
	}
};

class Sandbox : public Eternal::Application
{
public:
	Sandbox() 
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Eternal::ImGuiLayer());
	}

	~Sandbox() 
	{

	}
};

Eternal::Application* Eternal::CreateApplication()
{
	return new Sandbox();
}