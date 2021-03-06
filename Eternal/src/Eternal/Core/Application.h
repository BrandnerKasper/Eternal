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
		Application(const std::string& name = "Eternal App");
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(SharedPtr<Layer> layer);
		void PushOverlay(SharedPtr<Layer> layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
		inline ImGuiLayer& GetImGuiLayer() { return *m_ImGuiLayer; }
		void Close();
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		UniquePtr<Window> m_Window;
		SharedPtr<ImGuiLayer> m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication(); // to be defined in a client
}

