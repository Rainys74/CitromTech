#pragma once

#include "Core.h"
#include "Renderer/Camera.h"
#include "Math/Transform.h"

class EditorCamera
{
public:
	static EditorCamera* Get()
	{
		static EditorCamera instance;
		return &instance;
	}
public:
	EditorCamera();
	~EditorCamera() = default;

	void Update(float deltaTime);

	FORCE_INLINE bool IsActive() const { return m_Active; }
	inline void SetActive(bool active = true) { m_Active = active; }

	FORCE_INLINE Citrom::Camera* GetCamera() { return &m_Camera; }
	FORCE_INLINE Citrom::Math::Transform* GetTransform() { return &m_Transform; }
private:
	void ProcessInput(float deltaTime);
	void UpdateRotation(float deltaTime);
	void UpdateMovement(float deltaTime);
private:
	Citrom::Camera m_Camera;
	Citrom::Math::Transform m_Transform;

	float32 m_Boost = 1.0f;

	bool m_Active = true;

	float m_Yaw = 0.0f;
	float m_Pitch = 0.0f;
};