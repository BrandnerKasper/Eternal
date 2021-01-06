#pragma once

#include <glm/glm.hpp>

namespace Eternal {

	class Camera
	{
	public:
		Camera(const glm::mat4& projection)
			: m_Projection(projection) {}

		Camera() = default;

		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return m_Projection; }
	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);
	};

	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		const glm::vec3& GetPosition() const { return m_Position; }
		float GetRotation() const { return m_Rotation; }

		void SetPosition(const glm::vec3& position) { m_Position = position; UpdateView(); }
		void SetRotation(float rotation) { m_Rotation = rotation; UpdateView(); }
		void SetFOV(float zoomLevel) { m_FOV = zoomLevel; UpdateProjection(); }

		inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

	private:
		void UpdateProjection();
		void UpdateView();

	private:
		float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 10000.0f;
		glm::mat4 m_ViewMatrix;


		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;

		float m_ViewportWidth = 1280, m_ViewportHeight = 720;
	};
}