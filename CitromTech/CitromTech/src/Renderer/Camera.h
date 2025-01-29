#pragma once

#include "Math/MathCommon.h"
#include "Math/Matrix4x4.h"

namespace Citrom
{
	class Camera
	{
	public:
		enum class ProjectionType
		{
			Perspective,
			Orthographic
		};
	public:
		Camera() { RecalculateProjection(); }
		~Camera() = default;

		void SetPerspective(float32 verticalFOV, float32 nearClip, float32 farClip)
		{
			m_ProjectionType = ProjectionType::Perspective;
			m_PerspectiveFOV = verticalFOV;
			m_PerspectiveNear = nearClip;
			m_PerspectiveFar = farClip;

			RecalculateProjection();
		}
		void SetOrthographic(float32 size, float32 nearClip, float32 farClip)
		{
			m_ProjectionType = ProjectionType::Orthographic;
			m_OrthographicSize = size;
			m_OrthographicNear = nearClip;
			m_OrthographicFar = farClip;

			RecalculateProjection();
		}

		void SetViewportSize(uint32_t width, uint32_t height) { m_AspectRatio = (float32)width / (float32)height; RecalculateProjection(); }

		FORCE_INLINE float32 GetPerspectiveVerticalFOV() const { return m_PerspectiveFOV; }
		void SetPerspectiveVerticalFOV(float32 verticalFov) { m_PerspectiveFOV = verticalFov; RecalculateProjection(); }
		FORCE_INLINE float32 GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		void SetPerspectiveNearClip(float32 nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }
		FORCE_INLINE float32 GetPerspectiveFarClip() const { return m_PerspectiveFar; }
		void SetPerspectiveFarClip(float32 farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }

		FORCE_INLINE float32 GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float32 size) { m_OrthographicSize = size; RecalculateProjection(); }
		FORCE_INLINE float32 GetOrthographicNearClip() const { return m_OrthographicNear; }
		void SetOrthographicNearClip(float32 nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }
		FORCE_INLINE float32 GetOrthographicFarClip() const { return m_OrthographicFar; }
		void SetOrthographicFarClip(float32 farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }

		FORCE_INLINE ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }
	private:
		void RecalculateProjection()
		{
			if (m_ProjectionType == ProjectionType::Perspective)
			{
				//m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
				m_Projection.Perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
			}
			else
			{
				float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
				float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
				float orthoBottom = -m_OrthographicSize * 0.5f;
				float orthoTop = m_OrthographicSize * 0.5f;

				//m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
				m_Projection.Orthographic(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
			}
		}
	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float32 m_PerspectiveFOV = Math::DegreesToRadians(45.0f);
		float32 m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

		float32 m_OrthographicSize = 10.0f;
		float32 m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		float32 m_AspectRatio = 0.0f;

		Math::Matrix4x4 m_Projection = Math::Matrix4x4::Identity();
	};
}