#pragma once

#include "Eternal/Renderer/Camera.h"

namespace Eternal {

	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetViewportSize(uint32_t width, uint32_t height);

		float GetOrthographicSize() const { return m_OrthograpicSize; }
		void SetOrthographicSize(float size) { m_OrthograpicSize = size; RecalculateProjection(); }

		float GetOrthographicNearClip() const { return m_OrthographicNear; }
		void SetOrthographicNearClip(float size) { m_OrthographicNear = size; RecalculateProjection(); }

		float GetOrthographicFarClip() const { return m_OrthograpghicFar; }
		void SetOrthographicFarClip(float size) { m_OrthograpghicFar = size; RecalculateProjection(); }
	private:
		void RecalculateProjection();
	private:
		float m_OrthograpicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthograpghicFar = 1.0f;

		float m_AspectRatio= 0.0f;
	};
}