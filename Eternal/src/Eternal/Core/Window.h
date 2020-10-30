#pragma once

#include "ETpch.h"

#include "Eternal/Core/Core.h"
#include "Eternal/Events/Event.h"

namespace Eternal {

	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Eternal Engine",
					 uint32_t width = 1920,
					 uint32_t height = 1080)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	//Interface representating a desktop system based Window
	class ETERNAL_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		//Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}