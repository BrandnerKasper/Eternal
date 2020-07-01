#pragma once

#include "../Core.h"
#include "Eternal/Core.h" //hmmmmmm

#include <string>
#include <functional>

namespace Eternal {

	/*
		Events for now are blocking, meaning when a event is triggered it
		has to be dealt right away!
		Todo: buffer events!
	*/

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4),
	};


}