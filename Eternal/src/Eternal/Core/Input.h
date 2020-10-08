#pragma once

#include "Eternal/Core/Core.h"

namespace Eternal {

	class Input
	{
	public:
		static bool IsKeyPressed(int keycode);

		static bool IsMouseButtonPressed(int button);
		static std::pair<double, double> GetMousePosition();
		static double GetMouseX();
		static double GetMouseY();
	};
}