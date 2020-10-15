#include "ETpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Eternal {

	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_OrthograpicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthograpghicFar = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio	= (float)width / (float)height;
		ET_CORE_INFO("AspectRatio: {0}", m_AspectRatio);

		RecalculateProjection();
	}
	void SceneCamera::RecalculateProjection()
	{
		float orthoLeft = -m_OrthograpicSize * m_AspectRatio * 0.5f;
		float orthoRight = m_OrthograpicSize * m_AspectRatio * 0.5f;
		float orthoBottom = -m_OrthograpicSize * 0.5f;
		float orthoTop = m_OrthograpicSize * 0.5f;

		m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthograpghicFar);
	}
}