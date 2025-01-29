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
	// input detect!

	if (Input::SimpleInput::GetMouseButton(Input::MouseButton::RightButton))
		m_Transform.position.x += 0.1f * deltaTime;
}