#include "Application.h"
#include <stdio.h>

#include "Eternal/Events/ApplicationEvent.h"
#include "Eternal/Log.h"

namespace Eternal {

	Application::Application() 
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		//example of event!
		WindowResizeEvent e(1200, 700);
		if (e.IsInCategory(EventCategoryInput))
		{
			ET_WARN(e);
		}
		else
		{
			ET_ERROR(e);
		}

		while (true) {
			printf("Slaying Demons...");
		};
	}

}