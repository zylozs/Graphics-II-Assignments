/*
*	Vincent Loignon
*	Assignment 3
*	InputSystem.h
*	I certify that this assignment is entirely our own work.
*/

#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include "Keys.h"
#include "MouseKeys.h"
#include "EventDispatcher.h"

enum class InputEventType
{
	KEY_DOWN,
	KEY_UP,
	MOUSE_DOWN,
	MOUSE_UP,
	KEY_ALL,
	MOUSE_ALL,
	ALL
};

class InputSystem : public EventDispatcher
{
private:
	IDirectInput8 *m_DirectInput;
	IDirectInputDevice8 *m_Keyboard;
	IDirectInputDevice8 *m_Mouse;
	HWND m_HWND;

	// Current Key & Mouse State
	unsigned char m_KeyboardState[256];
	DIMOUSESTATE m_MouseState;

	unsigned char m_PreviousKeyboardState[256];
	DIMOUSESTATE m_PreviousMouseState;

	bool m_KeysPressed[(int)Keys::MAX];
	bool m_MouseKeysPressed[(int)MouseKeys::MAX];

	int m_MouseX,
		m_MouseY;

	bool m_DebugMode;

	bool m_Initialized;
	bool m_UseKeyDownEvents;
	bool m_UseKeyUpEvents;
	bool m_UseMouseDownEvents;
	bool m_UseMouseUpEvents;

	bool readKeyboard();
	bool readMouse();
	void processInput();
	void sendKeyEvents();
	void sendMouseEvents();

	bool isKeyDown(Keys key, unsigned char state[]);
	bool isMouseDown(MouseKeys key, DIMOUSESTATE& state);

public:
	InputSystem(bool useKeyEvents, bool useMouseEvents); // Constructor
	~InputSystem(); // Destructor

	// Enable Key/Mouse Pressed & Released Events
	void enableEvents(InputEventType type);

	// Disable Key/Mouse Pressed & Released Events
	void disableEvents(InputEventType type);

	// Checks to see if specific events are enabled
	bool isEventsEnabled(InputEventType type);
	

	bool isDebugMode() { return m_DebugMode; }
	void setDebugMode(bool value) { m_DebugMode = value; }

	bool initialize(HINSTANCE hinstance, HWND hwnd);
	bool update();
	virtual void dispose();

	void getMousePos(int& mouseX, int& mouseY);
	int getMouseX() { return m_MouseX; }
	int getMouseY() { return m_MouseY; }
	float getMouseDX() { return (float)m_MouseState.lX; }
	float getMouseDY() { return (float)m_MouseState.lY; }
	float getMouseDZ() { return (float)m_MouseState.lZ; }

	bool isKeyDown(Keys key);
	bool isMouseDown(MouseKeys key);
};

extern InputSystem* g_Input;

#endif