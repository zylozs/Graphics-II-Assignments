/*
*	Vincent Loignon
*	Assignment 4
*	MouseEvent.h
*	I certify that this assignment is entirely our own work.
*/

#ifndef MOUSE_EVENT_H
#define MOUSE_EVENT_H

#include "Event.h"
#include "MouseKeys.h"

const Events::EventType MOUSE_DOWN = "MOUSE_DOWN";
const Events::EventType MOUSE_UP = "MOUSE_UP";

class MouseEvent : public Event
{
protected:
	MouseKeys m_MouseKey;
	int m_X;
	int m_Y;

public:
	MouseEvent(MouseKeys key, Events::EventType type, int mouseX, int mouseY); // Constructor
	~MouseEvent(); // Destructor

	// Accessors
	MouseKeys getKey() const { return m_MouseKey; }
	int getMouseX() const { return m_X; }
	int getMouseY() const { return m_Y; }

	virtual void dispose();
};

#endif