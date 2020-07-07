#pragma once

#include "Core.h";

#include "Window.h"
#include "Eternal/LayerStack.h"
#include "Eternal/Events/Event.h"
#include "Eternal/Events/ApplicationEvent.h"

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
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	Application* CreateApplication(); // to be defined in a client
}

