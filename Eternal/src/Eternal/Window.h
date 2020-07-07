#pragma once

#include "ETpch.h"

#include "Eternal/Core.h"
#include "Eternal/Events/Event.h"

namespace Eternal {

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Eternal Engine",
					 unsigned int width = 1280,
					 unsigned int height = 720)
			: Title(title), Width(width), Height(height){}
	};

	//Interface representating a desktop system based Window
	class ETERNAL_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		//Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}