#include "EditorCamera.h"

#include "Input/KeyMouseInputSystem.h"

using namespace Citrom;
using namespace Math;

static constexpr float moveSpeed = 1.0f;
static constexpr float boostSpeed = 10.0f;

EditorCamera::EditorCamera()
{
	const float32 aspectRatio = (float32)CameraViewport::Get()->GetViewportWidth() / (float32)CameraViewport::Get()->GetViewportHeight();
	m_Camera.SetPerspective(Math::FovXToFovY(70.0f, aspectRatio), 0.01f, 1000.0f);
}

void EditorCamera::Update(float deltaTime)
{
	if (Input::SimpleInput::GetKeyDown(Input::KeyCode::F9))
		m_Active = !m_Active;

	if (!m_Active)
		return;

	if (Input::SimpleInput::GetMouseButton(Input::MouseButton::RightButton))
	{
		ProcessInput(deltaTime);
	}
	else if (Input::SimpleInput::GetMouseButton(Input::MouseButton::MiddleButton))
	{
		m_Transform.position.x += Input::SimpleInput::GetMouseDeltaX() * deltaTime;
		m_Transform.position.y += Input::SimpleInput::GetMouseDeltaY() * deltaTime;
	}
}

void EditorCamera::ProcessInput(float deltaTime)
{
	/*if (Input::SimpleInput::GetMouseButtonDown(Input::MouseButton::RightButton))
	{
		Input::SimpleInput::SetCursorVisible(false);
		Input::SimpleInput::SetCursorLocked(true);
	}

	if (Input::SimpleInput::GetMouseButtonUp(Input::MouseButton::RightButton))
	{
		Input::SimpleInput::SetCursorVisible(true);
		Input::SimpleInput::SetCursorLocked(false);
	}*/

	UpdateRotation(deltaTime);
	UpdateMovement(deltaTime);
}

void EditorCamera::UpdateRotation(float deltaTime)
{
	float mouseX = Input::SimpleInput::GetMouseDeltaX();
	float mouseY = Input::SimpleInput::GetMouseDeltaY();

	float m_MouseSensitivity = 0.1f;
	bool m_InvertY = false;

	float sensitivity = m_MouseSensitivity;
	if (m_InvertY)
		mouseY = -mouseY;

	m_Yaw += mouseX * sensitivity;
	m_Pitch += mouseY * sensitivity;

	// Clamp pitch to prevent flipping
	if (m_Pitch > 89.0f) m_Pitch = 89.0f;
	if (m_Pitch < -89.0f) m_Pitch = -89.0f;

	CT_WARN("Pitch: {}", m_Pitch);
	CT_WARN("Yaw: {}", m_Yaw);
	m_Transform.editorEulerAngles = Vector3(DegreesToRadians(m_Pitch), DegreesToRadians(m_Yaw), 0.0f); // testing!
	m_Transform.rotation = Quaternion::Euler(DegreesToRadians(m_Pitch), DegreesToRadians(m_Yaw), 0.0f);
	CT_ERROR("Quaternion: {}", m_Transform.rotation.ToString());
}

void EditorCamera::UpdateMovement(float deltaTime)
{
	Vector3 direction;

	if (Input::SimpleInput::GetKey(Input::KeyCode::W))
		direction += Vector3::Forward();
	if (Input::SimpleInput::GetKey(Input::KeyCode::S))
		direction += Vector3::Back();
	if (Input::SimpleInput::GetKey(Input::KeyCode::A))
		direction += Vector3::Left();
	if (Input::SimpleInput::GetKey(Input::KeyCode::D))
		direction += Vector3::Right();
	if (Input::SimpleInput::GetKey(Input::KeyCode::Q))
		direction += Vector3::Down();
	if (Input::SimpleInput::GetKey(Input::KeyCode::E))
		direction += Vector3::Up();

	if (direction != Vector3::Zero())
	{
		CT_VERBOSE("DIRECTION BEFORE ROTATION: {}", direction.ToString());
		direction = m_Transform.rotation * direction; // Apply rotation
		CT_VERBOSE("DIRECTION AFTER ROTATION: {}", direction.ToString());
		float speed = (Input::SimpleInput::GetKey(Input::KeyCode::LShift)) ? 10.0f : 1.0f;
		speed *= m_Boost;

		m_Transform.position += direction * speed * deltaTime;
	}

	// Adjust boost speed with mouse scroll
	//m_Boost += Input::SimpleInput::GetMouseScrollDelta() * 0.2f;
	//m_Boost = std::max(1.0f, m_Boost);
}
