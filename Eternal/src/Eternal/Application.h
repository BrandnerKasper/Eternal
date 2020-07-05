#pragma once

#include "Core.h";
#include "Events/Event.h"
#include "Window.h"

namespace Eternal {

	class ETERNAL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	Application* CreateApplication(); // to be defined in a client
}

