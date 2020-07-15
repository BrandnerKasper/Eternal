#include "ETpch.h"

#include <Eternal.h>

#include "imgui/imgui.h"

class ExampleLayer : public Eternal::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		//ET_INFO("ExampleLayer::Update");
		if (Eternal::Input::IsKeyPressed(ET_KEY_TAB)) //should be ET_KEY_TAB
			ET_TRACE("Tab pressed (poll)");
	}

	void OnEvent(Eternal::Event& event) override
	{
		//ET_TRACE("{0}", event);
		if (event.GetEventType() == Eternal::EventType::KeyPressed)
		{
			Eternal::KeyPressedEvent& e = (Eternal::KeyPressedEvent&)event;
			ET_INFO("{0}, {1}", (char)e.GetKeyCode(), e.GetRepeatCount());
			if (e.GetKeyCode() == ET_KEY_TAB)
				ET_WARN("oh my god you pressed the tab button!!! (event will happen... soon)");
		}
	}

	virtual void OnImGuiRender() override //Test that sandbox can use ImGui itself <3
	{
		ImGui::Begin("Test");
		ImGui::Text("hello");
		ImGui::End();
	}
};

class Sandbox : public Eternal::Application
{
public:
	Sandbox() 
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox() 
	{

	}
};

Eternal::Application* Eternal::CreateApplication()
{
	return new Sandbox();
}