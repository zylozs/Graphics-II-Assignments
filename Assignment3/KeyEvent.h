/* Author:      Vincent Loignon
   Class:       CSI 310_01
   Assignment:  Midterm Assignment
   Assigned:    2/07/14
   Due:         3/10/14

Certification of Authenticity:
I certify that this assignment is entirely my own work.
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