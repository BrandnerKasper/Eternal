#include "ETpch.h"

#include "Application.h"
#include <stdio.h>

#include "Eternal/Events/ApplicationEvent.h"

#include<GLFW/glfw3.h>

namespace Eternal {

	Application::Application() 
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		//example of event!
		/*WindowResizeEvent e(1200, 700);
		if (e.IsInCategory(EventCategoryInput))
		{
			ET_WARN(e);
		}
		else
		{
			ET_ERROR(e);
		}*/

		while (m_Running) {
			printf("Slaying Demons...");
			glClearColor(0.2, 0.8, 0.2, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		};
	}

}