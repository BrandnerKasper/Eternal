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

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};
}