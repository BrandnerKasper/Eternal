#pragma once

#include "Eternal/Layer.h"

#include "Eternal/Events/ApplicationEvent.h"
#include "Eternal/Events/MouseEvent.h"
#include "Eternal/Events/KeyEvent.h"


namespace Eternal {

	class ETERNAL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);
	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e); //maybe later
		bool OnWindowResizedEvent(WindowResizeEvent& e);

	private:
		float m_Time = 0.0f;
	};
}