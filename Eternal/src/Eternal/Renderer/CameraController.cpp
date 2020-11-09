#include "ETpch.h"
#include "CameraController.h"

#include "Eternal/Core/Input.h"
#include "Eternal/Core/KeyCodes.h"

namespace Eternal {

	EditorCameraController::EditorCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_RotationEnabled(rotation)
	{
		
	}

	void EditorCameraController::OnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(ET_KEY_LEFT) || Input::IsKeyPressed(ET_KEY_A))
			m_CameraPosition.x -= m_CameraSpeed * ts;

		else if (Input::IsKeyPressed(ET_KEY_RIGHT) || Input::IsKeyPressed(ET_KEY_D))
			m_CameraPosition.x += m_CameraSpeed * ts;

		if (Input::IsKeyPressed(ET_KEY_UP) || Input::IsKeyPressed(ET_KEY_W))
			m_CameraPosition.y += m_CameraSpeed * ts;

		else if (Input::IsKeyPressed(ET_KEY_DOWN) || Input::IsKeyPressed(ET_KEY_S))
			m_CameraPosition.y -= m_CameraSpeed * ts;

		if (m_RotationEnabled)
		{
			if (Input::IsKeyPressed(ET_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;

			else if (Input::IsKeyPressed(ET_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);
		m_CameraSpeed = m_ZoomLevel * 2.0f;
	}

	void EditorCameraController::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width/(float)height;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	void EditorCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(ET_BIND_EVENT_FN(EditorCameraController::OnMouseScrolled));
	}

	bool EditorCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.5f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}
}