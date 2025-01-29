#include "EditorCamera.h"

#include "Input/KeyMouseInputSystem.h"

using namespace Citrom;

EditorCamera::EditorCamera()
{
	const float32 aspectRatio = (float32)CameraViewport::Get()->GetViewportWidth() / (float32)CameraViewport::Get()->GetViewportHeight();
	m_Camera.SetPerspective(Math::FovXToFovY(70.0f, aspectRatio), 0.01f, 1000.0f);
}

void EditorCamera::Update(float deltaTime)
{
	if (Input::SimpleInput::GetKeyDown(Input::KeyCode::F9))
		m_Active = !m_Active;

	if (Input::SimpleInput::GetMouseButton(Input::MouseButton::RightButton))
	{
		constexpr float moveSpeed = 0.1f;

		// TODO: use 3D and orientation instead of 2D!
		if (Input::SimpleInput::GetKey(Input::KeyCode::A))
			m_Transform.position.x -= moveSpeed * deltaTime;
		if (Input::SimpleInput::GetKey(Input::KeyCode::D))
			m_Transform.position.x += moveSpeed * deltaTime;

		if (Input::SimpleInput::GetKey(Input::KeyCode::S))
			m_Transform.position.y -= moveSpeed * deltaTime;
		if (Input::SimpleInput::GetKey(Input::KeyCode::W))
			m_Transform.position.y += moveSpeed * deltaTime;
	}
	else if (Input::SimpleInput::GetMouseButton(Input::MouseButton::MiddleButton))
	{
		m_Transform.position.x += Input::SimpleInput::GetMouseDeltaX() * deltaTime;
		m_Transform.position.y += Input::SimpleInput::GetMouseDeltaY() * deltaTime;
	}
}