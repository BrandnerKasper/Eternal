#pragma once

#include "Core.h";
#include "Events/Event.h"

namespace Eternal {

	class ETERNAL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* CreateApplication(); // to be defined in a client
}

