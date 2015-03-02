/*
*	Vincent Loignon
*	Assignment 3
*	InputSystem.cpp
*	I certify that this assignment is entirely our own work.
*/

#include "InputSystem.h"
#include "Event.h"
#include "MouseEvent.h"
#include "KeyEvent.h"
#include <iostream>

InputSystem* g_Input = NULL;

/*   Purpose:  Constructor
 *       Pre:  bool useKeyEvents, bool useMouseEvents
 *      Post:  None
 ****************************************************************/
InputSystem::InputSystem(bool useKeyEvents, bool useMouseEvents)
{
	m_Initialized = false;
	m_UseKeyDownEvents = useKeyEvents;
	m_UseKeyUpEvents = useKeyEvents;
	m_UseMouseDownEvents = useMouseEvents;
	m_UseMouseUpEvents = useMouseEvents;
	m_DebugMode = false;
}

/*   Purpose:  Destructor
 *       Pre:  None
 *      Post:  None
 ****************************************************************/
InputSystem::~InputSystem()
{

}

/*   Purpose:  This will initialize the input system
 *       Pre:  HINSTANCE hinstance, HWND hwnd
 *      Post:  Returns whether or not it was successful
 ****************************************************************/
bool InputSystem::initialize(HINSTANCE hinstance, HWND hwnd)
{
	try
	{
		HRESULT result;

		m_HWND = hwnd;

		// Initialize the location of the mouse on the screen
		m_MouseX = 0;
		m_MouseY = 0;

		// Initialize Keys Pressed array
		for (int i = 0; i < (int)Keys::MAX; i++)
			m_KeysPressed[i] = false;

		// Initialize Mouse Keys Pressed array
		for (int i = 0; i < (int)MouseKeys::MAX; i++)
			m_MouseKeysPressed[i] = false;

		// Initialize the main direct input interface
		result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_DirectInput, NULL);
		if (FAILED(result))
			throw "Direct Input Interface";

		// Initialize the direct input interface for the keyboard
		result = m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_Keyboard, NULL);
		if (FAILED(result))
			throw "Direct Input Interface Keyboard";

		// Set the data format. In this case since it is a keyboard we can use the predine data format
		result = m_Keyboard->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(result))
			throw "Keyboard Data Format";

		// Set the cooperative level of the keyboard to not share with other programs
		result = m_Keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		if (FAILED(result))
			throw "Keyboard Cooperative Level";

		// Now acquire the keyboard
		result = m_Keyboard->Acquire();
		if (FAILED(result))
			throw "Keyboard Acquire";

		// Initialize the direct input interface for the mouse
		result = m_DirectInput->CreateDevice(GUID_SysMouse, &m_Mouse, NULL);
		if (FAILED(result))
			throw "Direct Input Interface Mouse";

		// Set the data format for the mouse using the pre-defined mouse data format
		result = m_Mouse->SetDataFormat(&c_dfDIMouse);
		if (FAILED(result))
			throw "Mouse Data Format";

		// Set the cooperative level of the mouse to share with other programs
		result = m_Mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(result))
			throw "Mouse Cooperative Level";

		// Acquire the mouse
		result = m_Mouse->Acquire();
		if (FAILED(result))
			throw "Mouse Acquire";
	}
	catch (std::string e)
	{
		MessageBox(hwnd, "There was a problem initializing the Input System.", "Exception", MB_OK);
		return false;
	}
	catch (...)
	{
		MessageBox(hwnd, "An Unhandled Exception has occurred while initializing the Input System.", "Unhandled Exception", MB_OK);
		return false;
	}

	m_Initialized = true;
	return true;
}

/*   Purpose:  This will clean up any memory for the input system
 *       Pre:  None
 *      Post:  Returns whether or not it was successful
 ****************************************************************/
void InputSystem::dispose()
{
	EventDispatcher::dispose();

	try
	{
		// Release the mouse
		if (m_Mouse)
		{
			m_Mouse->Unacquire();
			m_Mouse->Release();
			m_Mouse= NULL;
		}

		// Release the keyboard
		if (m_Keyboard)
		{
			m_Keyboard->Unacquire();
			m_Keyboard->Release();
			m_Keyboard = NULL;
		}

		// Release the main interface to direct input
		if (m_DirectInput)
		{
			m_DirectInput->Release();
			m_DirectInput = NULL;
		}
	}
	catch (...)
	{
		MessageBox(m_HWND, "An Unhandled Exception has occurred while Disposing the Input System.", "Unhandled Exception", MB_OK);
		return;
	}

	m_Initialized = false;
}

/*   Purpose:  This will update the input system
 *       Pre:  None
 *      Post:  Returns whether or not it was successful
 ****************************************************************/
bool InputSystem::update()
{
	bool result;

	// Set up the previous keyboard and mouse states
	for (int i = 0; i < 256; i++)
		m_PreviousKeyboardState[i] = m_KeyboardState[i];

	m_PreviousMouseState = m_MouseState;

	// Read the current state of the keyboard
	result = readKeyboard();
	if (!result)
		return false;

	// Read the current state of the mouse
	result = readMouse();
	if (!result)
		return false;

	// Process the changes in the mouse and keyboard
	processInput();

	return true;
}

/*   Purpose:  This will read the keyboard
 *       Pre:  None
 *      Post:  Returns whether or not it was successful
 ****************************************************************/
bool InputSystem::readKeyboard()
{
	HRESULT result;

	// Read the keyboard device
	result = m_Keyboard->GetDeviceState(sizeof(m_KeyboardState), (LPVOID)&m_KeyboardState);
	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			m_Keyboard->Acquire();
		else
			return false;
	}

	return true;
}

/*   Purpose:  This will read the mouse
 *       Pre:  None
 *      Post:  Returns whether or not it was successful
 ****************************************************************/
bool InputSystem::readMouse()
{
	HRESULT result;

	// Read the mouse device
	result = m_Mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_MouseState);
	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			m_Mouse->Acquire();
		else
			return false;
	}

	return true;
}

/*   Purpose:  This will process the input from the keyboard and mouse
 *       Pre:  None
 *      Post:  None
 ****************************************************************/
void InputSystem::processInput()
{
	// Update the location of the mouse cursor based on the change of the mouse location during the frame
	m_MouseX += m_MouseState.lX;
	m_MouseY += m_MouseState.lY;

	// Send Events
	if (m_UseKeyDownEvents || m_UseKeyUpEvents)
		sendKeyEvents();
	
	if (m_UseMouseDownEvents || m_UseMouseUpEvents)
		sendMouseEvents();
}

/*   Purpose:  This will return the mouse location
 *       Pre:  int& mouseX, int& mouseY
 *      Post:  None
 ****************************************************************/
void InputSystem::getMousePos(int& mouseX, int& mouseY)
{
	mouseX = m_MouseX;
	mouseY = m_MouseY;
}

/*   Purpose:  This will send all key events that the system is setup for
 *       Pre:  None
 *      Post:  None
 ****************************************************************/
void InputSystem::sendKeyEvents()
{
	KeyEvent* keyEvent = NULL;

	// Output all Key Events
	for (int i = 0; i < (int)Keys::MAX; i++)
	{
		// If a key is currently being pressed
		if (isKeyDown((Keys)i))
		{
			m_KeysPressed[i] = true;

			if (m_UseKeyDownEvents)
			{
				keyEvent = New KeyEvent((Keys)i, KEY_DOWN);

				if (m_DebugMode)
					std::cout << "Key is pressed!\n";

				this->dispatchEvent(keyEvent);
			}
		}
		else if (m_KeysPressed[i]) // Checks to see if a key was just released and was pressed before
		{
			if (isKeyDown((Keys)i, m_PreviousKeyboardState))
			{
				m_KeysPressed[i] = false;

				if (m_UseKeyUpEvents)
				{
					keyEvent = New KeyEvent((Keys)i, KEY_UP);

					if (m_DebugMode)
						std::cout << "Key was released!\n";

					this->dispatchEvent(keyEvent);
				}
			}
		}
	}
}

/*   Purpose:  This will send all mouse events that the system is setup for
 *       Pre:  None
 *      Post:  None
 ****************************************************************/
void InputSystem::sendMouseEvents()
{
	MouseEvent* mouseEvent = NULL;

	// Output all Mouse Events
	for (int i = 0; i < (int)MouseKeys::MAX; i++)
	{
		// If a mouse key is currently being pressed
		if (m_MouseState.rgbButtons[i] & 0x80)
		{
			m_MouseKeysPressed[i] = true;

			if (m_UseMouseDownEvents)
			{
				mouseEvent = New MouseEvent((MouseKeys)i, MOUSE_DOWN, m_MouseX, m_MouseY);

				if (m_DebugMode)
					std::cout << "mouse is pressed!\n";

				this->dispatchEvent(mouseEvent);
			}
		}
		else if (m_MouseKeysPressed[i]) // Checks to see if a mouse key was just released and was pressed before
		{
			if (m_PreviousMouseState.rgbButtons[i] & 0x80)
			{
				m_MouseKeysPressed[i] = false;

				if (m_UseMouseUpEvents)
				{
					mouseEvent = New MouseEvent((MouseKeys)i, MOUSE_UP, m_MouseX, m_MouseY);

					if (m_DebugMode)
						std::cout << "mouse was released!\n";

					this->dispatchEvent(mouseEvent);
				}
			}
		}
	}
}

/*   Purpose:  This will determine whether a certain key is pressed
 *       Pre:  Keys key, unsigned char state[]
 *      Post:  Returns whether or not it was successful
 ****************************************************************/
bool InputSystem::isKeyDown(Keys key, unsigned char state[])
{
	bool result;

	switch (key)
	{
		case Keys::A:
			result = (state[DIK_A] & 0x80 ? true : false);
			break;
		case Keys::B:
			result = (state[DIK_B] & 0x80 ? true : false);
			break;
		case Keys::C:
			result = (state[DIK_C] & 0x80 ? true : false);
			break;
		case Keys::D:
			result = (state[DIK_D] & 0x80 ? true : false);
			break;
		case Keys::E:
			result = (state[DIK_E] & 0x80 ? true : false);
			break;
		case Keys::F:
			result = (state[DIK_F] & 0x80 ? true : false);
			break;
		case Keys::G:
			result = (state[DIK_G] & 0x80 ? true : false);
			break;
		case Keys::H:
			result = (state[DIK_H] & 0x80 ? true : false);
			break;
		case Keys::I:
			result = (state[DIK_I] & 0x80 ? true : false);
			break;
		case Keys::J:
			result = (state[DIK_J] & 0x80 ? true : false);
			break;
		case Keys::K:
			result = (state[DIK_K] & 0x80 ? true : false);
			break;
		case Keys::L:
			result = (state[DIK_L] & 0x80 ? true : false);
			break;
		case Keys::M:
			result = (state[DIK_M] & 0x80 ? true : false);
			break;
		case Keys::N:
			result = (state[DIK_N] & 0x80 ? true : false);
			break;
		case Keys::O:
			result = (state[DIK_O] & 0x80 ? true : false);
			break;
		case Keys::P:
			result = (state[DIK_P] & 0x80 ? true : false);
			break;
		case Keys::Q:
			result = (state[DIK_Q] & 0x80 ? true : false);
			break;
		case Keys::R:
			result = (state[DIK_R] & 0x80 ? true : false);
			break;
		case Keys::S:
			result = (state[DIK_S] & 0x80 ? true : false);
			break;
		case Keys::T:
			result = (state[DIK_T] & 0x80 ? true : false);
			break;
		case Keys::U:
			result = (state[DIK_U] & 0x80 ? true : false);
			break;
		case Keys::V:
			result = (state[DIK_V] & 0x80 ? true : false);
			break;
		case Keys::W:
			result = (state[DIK_W] & 0x80 ? true : false);
			break;
		case Keys::X:
			result = (state[DIK_X] & 0x80 ? true : false);
			break;
		case Keys::Y:
			result = (state[DIK_Y] & 0x80 ? true : false);
			break;
		case Keys::Z:
			result = (state[DIK_Z] & 0x80 ? true : false);
			break;
		case Keys::KEY_0:
			result = (state[DIK_0] & 0x80 ? true : false);
			break;
		case Keys::KEY_1:
			result = (state[DIK_1] & 0x80 ? true : false);
			break;
		case Keys::KEY_2:
			result = (state[DIK_2] & 0x80 ? true : false);
			break;
		case Keys::KEY_3:
			result = (state[DIK_3] & 0x80 ? true : false);
			break;
		case Keys::KEY_4:
			result = (state[DIK_4] & 0x80 ? true : false);
			break;
		case Keys::KEY_5:
			result = (state[DIK_5] & 0x80 ? true : false);
			break;
		case Keys::KEY_6:
			result = (state[DIK_6] & 0x80 ? true : false);
			break;
		case Keys::KEY_7:
			result = (state[DIK_7] & 0x80 ? true : false);
			break;
		case Keys::KEY_8:
			result = (state[DIK_8] & 0x80 ? true : false);
			break;
		case Keys::KEY_9:
			result = (state[DIK_9] & 0x80 ? true : false);
			break;
		case Keys::NUM_PAD_0:
			result = (state[DIK_NUMPAD0] & 0x80 ? true : false);
			break;
		case Keys::NUM_PAD_1:
			result = (state[DIK_NUMPAD1] & 0x80 ? true : false);
			break;
		case Keys::NUM_PAD_2:
			result = (state[DIK_NUMPAD2] & 0x80 ? true : false);
			break;
		case Keys::NUM_PAD_3:
			result = (state[DIK_NUMPAD3] & 0x80 ? true : false);
			break;
		case Keys::NUM_PAD_4:
			result = (state[DIK_NUMPAD4] & 0x80 ? true : false);
			break;
		case Keys::NUM_PAD_5:
			result = (state[DIK_NUMPAD5] & 0x80 ? true : false);
			break;
		case Keys::NUM_PAD_6:
			result = (state[DIK_NUMPAD6] & 0x80 ? true : false);
			break;
		case Keys::NUM_PAD_7:
			result = (state[DIK_NUMPAD7] & 0x80 ? true : false);
			break;
		case Keys::NUM_PAD_8:
			result = (state[DIK_NUMPAD8] & 0x80 ? true : false);
			break;
		case Keys::NUM_PAD_9:
			result = (state[DIK_NUMPAD9] & 0x80 ? true : false);
			break;
		case Keys::F1:
			result = (state[DIK_F1] & 0x80 ? true : false);
			break;
		case Keys::F2:
			result = (state[DIK_F2] & 0x80 ? true : false);
			break;
		case Keys::F3:
			result = (state[DIK_F3] & 0x80 ? true : false);
			break;
		case Keys::F4:
			result = (state[DIK_F4] & 0x80 ? true : false);
			break;
		case Keys::F5:
			result = (state[DIK_F5] & 0x80 ? true : false);
			break;
		case Keys::F6:
			result = (state[DIK_F6] & 0x80 ? true : false);
			break;
		case Keys::F7:
			result = (state[DIK_F7] & 0x80 ? true : false);
			break;
		case Keys::F8:
			result = (state[DIK_F8] & 0x80 ? true : false);
			break;
		case Keys::F9:
			result = (state[DIK_F9] & 0x80 ? true : false);
			break;
		case Keys::F10:
			result = (state[DIK_F10] & 0x80 ? true : false);
			break;
		case Keys::F11:
			result = (state[DIK_F11] & 0x80 ? true : false);
			break;
		case Keys::F12:
			result = (state[DIK_F12] & 0x80 ? true : false);
			break;
		case Keys::ESCAPE:
			result = (state[DIK_ESCAPE] & 0x80 ? true : false);
			break;
		case Keys::TILDE:
			result = (state[DIK_GRAVE] & 0x80 ? true : false);
			break;
		case Keys::MINUS:
			result = (state[DIK_MINUS] & 0x80 ? true : false);
			break;
		case Keys::EQUALS:
			result = (state[DIK_EQUALS] & 0x80 ? true : false);
			break;
		case Keys::BACKSPACE:
			result = (state[DIK_BACKSPACE] & 0x80 ? true : false);
			break;
		case Keys::TAB:
			result = (state[DIK_TAB] & 0x80 ? true : false);
			break;
		case Keys::OPENBRACE:
			result = (state[DIK_LBRACKET] & 0x80 ? true : false);
			break;
		case Keys::CLOSEBRACE:
			result = (state[DIK_RBRACKET] & 0x80 ? true : false);
			break;
		case Keys::ENTER:
			result = (state[DIK_RETURN] & 0x80 ? true : false);
			break;
		case Keys::SEMICOLON:
			result = (state[DIK_SEMICOLON] & 0x80 ? true : false);
			break;
		case Keys::QUOTE:
			result = (state[DIK_APOSTROPHE] & 0x80 ? true : false);
			break;
		case Keys::BACKSLASH:
			result = (state[DIK_BACKSLASH] & 0x80 ? true : false);
			break;
		case Keys::COMMA:
			result = (state[DIK_COMMA] & 0x80 ? true : false);
			break;
		case Keys::PERIOD:
			result = (state[DIK_PERIOD] & 0x80 ? true : false);
			break;
		case Keys::SLASH:
			result = (state[DIK_SLASH] & 0x80 ? true : false);
			break;
		case Keys::SPACE:
			result = (state[DIK_SPACE] & 0x80 ? true : false);
			break;
		case Keys::INSERT:
			result = (state[DIK_INSERT] & 0x80 ? true : false);
			break;
		case Keys::DELETE:
			result = (state[DIK_DELETE] & 0x80 ? true : false);
			break;
		case Keys::HOME:
			result = (state[DIK_HOME] & 0x80 ? true : false);
			break;
		case Keys::END:
			result = (state[DIK_END] & 0x80 ? true : false);
			break;
		case Keys::PAGEUP:
			result = (state[DIK_PRIOR] & 0x80 ? true : false);
			break;
		case Keys::PAGEDOWN:
			result = (state[DIK_NEXT] & 0x80 ? true : false);
			break;
		case Keys::UP:
			result = (state[DIK_UP] & 0x80 ? true : false);
			break;
		case Keys::DOWN:
			result = (state[DIK_DOWN] & 0x80 ? true : false);
			break;
		case Keys::LEFT:
			result = (state[DIK_LEFT] & 0x80 ? true : false);
			break;
		case Keys::RIGHT:
			result = (state[DIK_RIGHT] & 0x80 ? true : false);
			break;
		case Keys::NUM_PAD_SLASH:
			result = (state[DIK_DIVIDE] & 0x80 ? true : false);
			break;
		case Keys::NUM_PAD_ASTERISK:
			result = (state[DIK_MULTIPLY] & 0x80 ? true : false);
			break;
		case Keys::NUM_PAD_MINUS:
			result = (state[DIK_SUBTRACT] & 0x80 ? true : false);
			break;
		case Keys::NUM_PAD_PLUS:
			result = (state[DIK_ADD] & 0x80 ? true : false);
			break;
		case Keys::NUM_PAD_DECIMAL:
			result = (state[DIK_DECIMAL] & 0x80 ? true : false);
			break;
		case Keys::NUM_PAD_ENTER:
			result = (state[DIK_NUMPADENTER] & 0x80 ? true : false);
			break;
		default:
			break;
	}

	return result;
}

/*   Purpose:  This will determine whether a certain key is pressed
 *       Pre:  Keys key
 *      Post:  Returns whether or not it was successful
 ****************************************************************/
bool InputSystem::isKeyDown(Keys key)
{
	return isKeyDown(key, m_KeyboardState);
}

bool InputSystem::isMouseDown(MouseKeys key, DIMOUSESTATE& state)
{
	return state.rgbButtons[(int)key] & 0x80;
}

bool InputSystem::isMouseDown(MouseKeys key)
{
	return isMouseDown(key, m_MouseState);
}

void InputSystem::enableEvents(InputEventType type)
{
	switch (type)
	{
		case InputEventType::KEY_DOWN:
			m_UseKeyDownEvents = true;
			break;
		case InputEventType::KEY_UP:
			m_UseKeyUpEvents = true;
			break;
		case InputEventType::MOUSE_DOWN:
			m_UseMouseDownEvents = true;
			break;
		case InputEventType::MOUSE_UP:
			m_UseMouseUpEvents = true;
			break;
		case InputEventType::KEY_ALL:
			m_UseKeyDownEvents = true;
			m_UseKeyUpEvents = true;
			break;
		case InputEventType::MOUSE_ALL:
			m_UseMouseDownEvents = true;
			m_UseMouseUpEvents = true;
			break;
		case InputEventType::ALL:
			m_UseKeyDownEvents = true;
			m_UseKeyUpEvents = true;
			m_UseMouseDownEvents = true;
			m_UseMouseUpEvents = true;
			break;
	}
}

void InputSystem::disableEvents(InputEventType type)
{
	switch (type)
	{
		case InputEventType::KEY_DOWN:
			m_UseKeyDownEvents = false;
			break;
		case InputEventType::KEY_UP:
			m_UseKeyUpEvents = false;
			break;
		case InputEventType::MOUSE_DOWN:
			m_UseMouseDownEvents = false;
			break;
		case InputEventType::MOUSE_UP:
			m_UseMouseUpEvents = false;
			break;
		case InputEventType::KEY_ALL:
			m_UseKeyDownEvents = false;
			m_UseKeyUpEvents = false;
			break;
		case InputEventType::MOUSE_ALL:
			m_UseMouseDownEvents = false;
			m_UseMouseUpEvents = false;
			break;
		case InputEventType::ALL:
			m_UseKeyDownEvents = false;
			m_UseKeyUpEvents = false;
			m_UseMouseDownEvents = false;
			m_UseMouseUpEvents = false;
			break;
	}
}

bool InputSystem::isEventsEnabled(InputEventType type)
{
	bool result = false;

	switch (type)
	{
		case InputEventType::KEY_DOWN:
			if (m_UseKeyDownEvents)
				result = true;
			break;
		case InputEventType::KEY_UP:
			if (m_UseKeyUpEvents)
				result = true;
			break;
		case InputEventType::MOUSE_DOWN:
			if (m_UseMouseDownEvents)
				result = true;
			break;
		case InputEventType::MOUSE_UP:
			if (m_UseMouseUpEvents)
				result = true;
			break;
		case InputEventType::KEY_ALL:
			if (m_UseKeyDownEvents && m_UseKeyUpEvents)
				result = true;
			break;
		case InputEventType::MOUSE_ALL:
			if (m_UseMouseDownEvents && m_UseMouseUpEvents)
				result = true;
			break;
		case InputEventType::ALL:
			if (m_UseKeyDownEvents && m_UseKeyUpEvents && m_UseMouseDownEvents && m_UseMouseUpEvents)
				result = true;
			break;
	}

	return result;
}