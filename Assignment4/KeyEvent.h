/*
*	Vincent Loignon
*	Assignment 4
*	KeyEvent.h
*	I certify that this assignment is entirely our own work.
*/

#ifndef KEY_EVENT_H
#define KEY_EVENT_H

#include "Keys.h"
#include "Event.h"

const Events::EventType KEY_DOWN = "KEY_DOWN";
const Events::EventType KEY_UP = "KEY_UP";

class KeyEvent : public Event
{
protected:
	Keys m_Key;

public:
	KeyEvent(Keys key, Events::EventType type); // Constructor
	~KeyEvent(); // Destructor

	// Accessors
	Keys getKey() const { return m_Key; }

	virtual void dispose();
};

#endif