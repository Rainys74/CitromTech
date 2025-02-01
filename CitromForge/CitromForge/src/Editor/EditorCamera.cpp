#include "EditorCamera.h"

#include "Input/KeyMouseInputSystem.h"

using namespace Citrom;
using namespace Math;

static constexpr float moveSpeed = 1.0f;
static constexpr float boostSpeed = 10.0f;

EditorCamera::EditorCamera()
{
	m_Camera.SetPerspective(DegreesToRadians(70.0f), 0.01f, 1000.0f);
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
	//m_Pitch = Math::Clamp(m_Pitch, -89.0f, 89.0f);
	
	CT_WARN("Pitch: {}", m_Pitch);
	CT_WARN("Yaw: {}", m_Yaw);
	m_Transform.eulerAnglesHint = Vector3(DegreesToRadians(m_Pitch), DegreesToRadians(m_Yaw), 0.0f); // testing!
	m_Transform.rotation = Quaternion::Euler(DegreesToRadians(m_Pitch), DegreesToRadians(m_Yaw), 0.0f);
	CT_ERROR("Quaternion: {}", m_Transform.rotation.ToString());

	/* 
	// get input
        Vector2 mouseXY = GetMouseInput();

        //Find current look rotation
        Vector3 rot = transform.localRotation.eulerAngles;
        desiredX = rot.y + mouseXY.x;

        //Rotate, and also make sure we dont over- or under-rotate.
        xRot -= mouseXY.y;
        xRot = Mathf.Clamp(xRot, -90f, 90f);

        //camera tilt similar to quake, but it uses player velocity as the bob
        sideSpeed = Vector3.Dot(pmove.rb.velocity, transform.right);
        zRot = -sideSpeed * gameSettings.cameraZBob;

        //Perform the rotations
        transform.localRotation = Quaternion.Euler(xRot, desiredX, zRot);
        orientation.transform.localRotation = Quaternion.Euler(0, desiredX, 0);
	*/

	/*float m_MouseSensitivity = 0.1f;
	bool m_InvertY = false;

	float sensitivity = m_MouseSensitivity;
	if (m_InvertY)
		mouseY = -mouseY;

	//Find current look rotation
	Vector3 rot = m_Transform.editorEulerAngles;
	rot.x = RadiansToDegrees(rot.x);
	rot.y = RadiansToDegrees(rot.y);
	rot.z = RadiansToDegrees(rot.z);
	m_Yaw = rot.y + (mouseX * sensitivity);
	//m_Yaw += mouseX * sensitivity;

	//Rotate, and also make sure we dont over- or under-rotate.
	m_Pitch -= (mouseY * sensitivity);
	m_Pitch = Math::Clamp(m_Pitch, -90.0f, 90.0f);

	//Perform the rotations
	m_Transform.editorEulerAngles = Vector3(DegreesToRadians(m_Pitch), DegreesToRadians(m_Yaw), 0.0f);
	m_Transform.rotation = Quaternion::Euler(DegreesToRadians(m_Pitch), DegreesToRadians(m_Yaw), 0.0f);

	CT_WARN("Pitch: {}", m_Pitch);
	CT_WARN("Yaw: {}", m_Yaw);*/
}

void EditorCamera::UpdateMovement(float deltaTime)
{
	float verticalInput = 0.0f;
	float horizontalInput = 0.0f;
	float depthInput = 0.0f;
	if (Input::SimpleInput::GetKey(Input::KeyCode::W))
		verticalInput = 1.0f;
	if (Input::SimpleInput::GetKey(Input::KeyCode::S))
		verticalInput = -1.0f;
	if (Input::SimpleInput::GetKey(Input::KeyCode::A))
		horizontalInput = -1.0f;
	if (Input::SimpleInput::GetKey(Input::KeyCode::D))
		horizontalInput = 1.0f;

	if (Input::SimpleInput::GetKey(Input::KeyCode::Q))
		depthInput = -1.0f;
	if (Input::SimpleInput::GetKey(Input::KeyCode::E))
		depthInput = 1.0f;

	float speed = (Input::SimpleInput::GetKey(Input::KeyCode::LShift)) ? 10.0f : 1.0f;
	speed *= m_Boost;
	
	//Vector3 moveDirection = orientation.forward * verticalInput + orientation.right * horizontalInput;
	Vector3 moveDirection = m_Transform.Forward() * verticalInput + m_Transform.Right() * horizontalInput + m_Transform.Up() * depthInput; // use m_Transform.Up() for moving orientationally, use Vector3::Up() for constantness
	m_Transform.position += moveDirection.Normalized() * speed * deltaTime;
	CT_ERROR("MOVEDIR: {}", moveDirection.ToString());

	// Adjust boost speed with mouse scroll
	m_Boost += Input::SimpleInput::GetMouseScrollDeltaY() * 0.1f;
	m_Boost = Math::Clamp(m_Boost, 0.1f, 5.0f);
	CT_WARN("M_BOOST!: {}", m_Boost);
}
