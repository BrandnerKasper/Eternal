#pragma once

#include <Eternal.h>

namespace Eternal {

	class GameLayer : public Layer
	{
	public:
		GameLayer();
		virtual ~GameLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& event) override;
	private:
		void HandleChangeScene();

		void NewScene();
		void Exit();

	private:

		Ref<Scene> m_ActiveScene;
		bool m_restart = false;

	};
}