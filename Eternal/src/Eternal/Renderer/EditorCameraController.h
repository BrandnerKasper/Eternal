#pragma once

#include "Eternal/Renderer/Camera.h"
#include "Eternal/Core/Timestep.h"

#include "Eternal/Events/ApplicationEvent.h"
#include "Eternal/Events/MouseEvent.h"

namespace Eternal {

	class EditorCameraController
	{
	public:
		EditorCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		EditorCamera& GetCamera() { return m_Camera; }
		const EditorCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }
		void OnViewportResize(uint32_t width, uint32_t height);
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
	private:
		float m_AspectRatio = 1.0f;
		float m_ZoomLevel = 5.0f;

		bool m_RotationEnabled;

		glm::vec3 m_CameraPosition = {0.0f, 0.0f, 20.0f};
		float m_CameraSpeed = 3.0f;
		float m_CameraRotation = 0.0f;
		float m_CameraRotationSpeed = 180.0f;

		EditorCamera m_Camera;
	};
}