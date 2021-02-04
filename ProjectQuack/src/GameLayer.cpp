#include "GameLayer.h"
#include "Eternal/Core/EntryPoint.h"
#include "Eternal/Scene/SceneSerializer.h"

namespace Eternal {

	GameLayer::GameLayer()
		: Layer("GameLayer")
	{
	}

	void GameLayer::OnAttach()
	{
		m_ActiveScene = CreateSharedPtr<Scene>();
		SceneSerializer serializer(m_ActiveScene);
		serializer.Deserialize("assets/scenes/TitelScreen.eternal");
		m_ActiveScene->SetPlay(true);
	}

	void GameLayer::OnDetach()
	{

	}

	void GameLayer::OnUpdate(Eternal::Timestep ts) // Max Frame Rate auf 60 cappen maybe!!
	{
		RenderCommand::SetClearColor({ 0.839, 1, 0.917, 0.8 });
		RenderCommand::Clear();

		if (m_restart == true)
		{
			m_ActiveScene->SetPlay(true);
			m_restart = false;
		}

		if (m_ActiveScene->GetPlay() == false)
		{
			m_restart = true;
		}

		m_ActiveScene->OnUpdate(ts);

		HandleChangeScene();
	}

	void GameLayer::OnEvent(Event& event)
	{
		m_ActiveScene->OnEvent(event);
	}

	void GameLayer::HandleChangeScene()
	{
		if (m_ActiveScene->GetChangeScene())
		{
			m_ActiveScene->ResetScripts();
			auto filepath = m_ActiveScene->GetChangeSceneFilepath();
			NewScene();
			if (!filepath.empty())
			{
				SceneSerializer serializer(m_ActiveScene);
				serializer.Deserialize(filepath);
			}

			//Set Play of new Scene
			m_ActiveScene->SetPlay(true);
		}
	}

	void GameLayer::NewScene()
	{
		m_ActiveScene = CreateSharedPtr<Scene>();
	}

	void GameLayer::Exit()
	{
		Application::Get().Close();
	}
}