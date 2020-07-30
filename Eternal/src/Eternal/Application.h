#pragma once

#include "Core.h"

#include "Window.h"
#include "Eternal/LayerStack.h"
#include "Eternal/Events/Event.h"
#include "Eternal/Events/ApplicationEvent.h"

#include "Eternal/ImGui/ImGuiLayer.h"

#include "Eternal/Renderer/Shader.h"
#include "Eternal/Renderer/Buffer.h"
#include "Eternal/Renderer/VertexArray.h"

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
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<Shader> m_BlueShader;
		std::shared_ptr<VertexArray> m_SquareVertexArray;

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication(); // to be defined in a client
}

