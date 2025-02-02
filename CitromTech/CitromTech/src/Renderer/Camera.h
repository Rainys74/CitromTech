#pragma once

#include "Math/MathCommon.h"
#include "Math/Matrix4x4.h"
#include "Math/Color.h"

#include "Events/EventSystem.h"
#include "Events/WindowEvents.h"

namespace Citrom
{
	class CameraViewport
	{
	public:
		static CameraViewport* Get()
		{
			static CameraViewport instance;
			return &instance;
		}

		inline void SetViewport(uint32 width, uint32 height)
		{
			m_Width = width;
			m_Height = height;
		}
		FORCE_INLINE uint32 GetViewportWidth() const
		{
			return m_Width;
		}
		FORCE_INLINE uint32 GetViewportHeight() const
		{
			return m_Height;
		}
	private:
		uint32 m_Width;
		uint32 m_Height;
	};

	class Camera
	{
	public:
		enum class ProjectionType
		{
			Perspective,
			Orthographic
		};
	public:
		Camera()
		{
			RecalculateProjection();

			// On Resize Callback
			m_WindowEventListener.OnEvent = [](const Event<WindowEvents>& event)
				{
					if (event.GetEventType() == WindowEvents::WindowResize)
					{
						const WindowResizeEvent& transformedEvent = (const WindowResizeEvent&)event;
						CameraViewport::Get()->SetViewport(transformedEvent.width, transformedEvent.height);
					}
				};
			EventBus::GetDispatcher<WindowEvents>()->AddListener(&m_WindowEventListener);
		}
		~Camera()
		{
			EventBus::GetDispatcher<WindowEvents>()->RemoveListener(&m_WindowEventListener);
		}

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

		FORCE_INLINE const Math::Matrix4x4& GetProjection() const { return m_Projection; }

		FORCE_INLINE float32 GetAspectRatio() { m_AspectRatio = (float32)CameraViewport::Get()->GetViewportWidth() / (float32)CameraViewport::Get()->GetViewportHeight(); return m_AspectRatio; }
	public:
		Math::ColorF32x4 clearColor = Math::ColorF32x4(0.6399964f, 0.7359726f, 0.8867924f);

		// Viewport multiplies these by width and height of the screen
		Math::Vector2 viewportSize = Math::Vector2(1.0f, 1.0f);
		Math::Vector2 viewportOffset = Math::Vector2(0.0f, 0.0f);
	private:
		void RecalculateProjection()
		{
			m_AspectRatio = (float32)CameraViewport::Get()->GetViewportWidth() / (float32)CameraViewport::Get()->GetViewportHeight();

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
		EventListener<WindowEvents> m_WindowEventListener;

		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float32 m_PerspectiveFOV = Math::DegreesToRadians(45.0f); // Vertical FOV as Radians
		float32 m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

		float32 m_OrthographicSize = 10.0f;
		float32 m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

		float32 m_AspectRatio = 0.0f;

		Math::Matrix4x4 m_Projection = Math::Matrix4x4::Identity();
	};
}