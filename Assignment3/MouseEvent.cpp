/*
*	Vincent Loignon
*	Assignment 3
*	MouseEvent.cpp
*	I certify that this assignment is entirely our own work.
*/

#include "MouseEvent.h"

/*   Purpose:  Constructor
 *       Pre:  MouseKeys key, EventType type, int mouseX, int mouseY
 *      Post:  None
 ****************************************************************/
MouseEvent::MouseEvent(MouseKeys key, Events::EventType type, int mouseX, int mouseY)
	: Event(type)
{
	m_MouseKey = key;
	m_X = mouseX;
	m_Y = mouseY;
}

/*   Purpose:  Destructor
 *       Pre:  None
 *      Post:  None
 ****************************************************************/
MouseEvent::~MouseEvent()
{

}

/*   Purpose:  This will clean up any memory for the MouseEvent
 *       Pre:  None
 *      Post:  None
 ****************************************************************/
void MouseEvent::dispose()
{

}