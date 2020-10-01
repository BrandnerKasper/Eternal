#pragma once

#include "Core.h"

#include "Window.h"
#include "Eternal/Core/LayerStack.h"
#include "Eternal/Events/Event.h"
#include "Eternal/Events/ApplicationEvent.h"

#include "Eternal/Core/Timestep.h"

#include "Eternal/ImGui/ImGuiLayer.h"

namespace Eternal {

	class ETERNAL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
		void Close();
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication(); // to be defined in a client
}

