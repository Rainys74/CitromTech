#pragma once

#include "KeyboardInput.h"
#include "MouseInput.h"

#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"

namespace Citrom::Input
{
	/*class //<unnamed>
	{
	public:
		void Foo();
	};*/

	// State based input system
	class SimpleInput
	{
	public:
		static inline SimpleInput* GetInstance()
		{
			static SimpleInput instance;
			return &instance;
		}

		static bool GetKey(KeyCode keyCode);

		static bool GetMouseButton(MouseButton mouseButton);

		static float GetMouseDeltaX();
		static float GetMouseDeltaY();
	private:
		static void TryInitialize();
	private:
		static float s_LastMousePositionX, s_LastMousePositionY;
		static float s_CurrentMousePositionX, s_CurrentMousePositionY;

		static bool s_KeysDown[(size_t)KeyCode::Count];
		static bool s_MButtonsDown[(size_t)MouseButton::Count];
		// or use states like CitromTech2D which requires updating, and calling get functions between the registering and update?

		static EventListener<KeyEvents> s_KeyEventsListener;
		static EventListener<MouseEvents> s_MouseEventsListener;

		static bool s_Initialized;
	};
}