#include "Application.h"
#include <stdio.h>

namespace Eternal {

	Application::Application() 
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (true) {
			printf("Slaying Demons...");
		};
	}

}