#pragma once

#include "Eternal/Renderer/Camera.h"
#include "Eternal/Core/Timestep.h"

#include "Eternal/Events/ApplicationEvent.h"
#include "Eternal/Events/MouseEvent.h"

namespace Eternal {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio = 1.0f;
		float m_ZoomLevel = 1.0f;

		bool m_RotationEnabled;

		glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};
		float m_CameraSpeed = 3.0f;
		float m_CameraRotation = 0.0f;
		float m_CameraRotationSpeed = 180.0f;

		OrthographicCamera m_Camera;
	};
}