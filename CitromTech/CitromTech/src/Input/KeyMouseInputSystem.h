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
		static bool GetKey(KeyCode keyCode);
		static bool GetKeyDown(KeyCode keyCode);
		static bool GetKeyUp(KeyCode keyCode);

		static bool GetMouseButton(MouseButton mouseButton);
		static bool GetMouseButtonDown(MouseButton mouseButton);
		static bool GetMouseButtonUp(MouseButton mouseButton);

		static float GetMouseDeltaX();
		static float GetMouseDeltaY();
	};

	class SimpleInputManager
	{
	public:
		static inline SimpleInputManager* GetInstance()
		{
			static SimpleInputManager instance;
			return &instance;
		}

		enum class KeyState
		{
			Down, // Held
			Up, // Released

			Pressed, // just pressed
			Released // just released
		};
	public:
		SimpleInputManager();
		~SimpleInputManager();

		void Update();
	private:
		EventListener<KeyEvents> m_KeyEventsListener;
		EventListener<MouseEvents> m_MouseEventsListener;
	};
}