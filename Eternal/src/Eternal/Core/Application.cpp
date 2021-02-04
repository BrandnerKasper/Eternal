#include "ETpch.h"

#include "Application.h"

#include "Eternal/Renderer/Renderer.h"

#include "Input.h"

namespace Eternal {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	/**
	* Initialize a application by creating a window context, init the renderer and push an ImGui layer
	*
	* @param std::string& The name of the application.
	* @return 
	*/
	Application::Application(const std::string& name)
	{
		ET_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = UniquePtr<Window>(Window::Create(WindowProps(name)));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init();

		m_ImGuiLayer = CreateUniquePtr<ImGuiLayer>();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
	}

	/// <summary>The main loop of the application using the frametime.</summary>
	void Application::Run()
	{
		while (m_Running) {
			//printf("Slaying Demons..."); GOOD TIMES :)
			float time = (float)Renderer::GetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized) {

				for (SharedPtr<Layer> layer : m_LayerStack)
					layer->OnUpdate(timestep);

				m_ImGuiLayer->Begin();
				for (SharedPtr<Layer> layer : m_LayerStack)
					layer->OnImGuiRender();
				m_ImGuiLayer->End();
			}
			m_Window->OnUpdate();
		};
	}

	/**
	* Cascades down all important events to the layers the application consists of. Handles the immediate resize and close of the given window.
	*
	* @param Event& The event reference.
	* @return
	*/
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			//if (e.Handled) -> Keep in Mind!
			//	break;
			(*it)->OnEvent(e);
			
		}
	}

	void Application::PushLayer(SharedPtr<Layer> layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(SharedPtr<Layer> overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}