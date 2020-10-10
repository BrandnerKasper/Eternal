#pragma once

#include "Eternal/Core/Layer.h"

#include "Eternal/Events/ApplicationEvent.h"
#include "Eternal/Events/MouseEvent.h"
#include "Eternal/Events/KeyEvent.h"


namespace Eternal {

	class ETERNAL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}