/*
*	Vincent Loignon
*	Assignment 4
*	KeyEvent.cpp
*	I certify that this assignment is entirely our own work.
*/
#include "KeyEvent.h"

/*   Purpose:  Constructor
 *       Pre:  Keys key, EventType type
 *      Post:  None
 ****************************************************************/
KeyEvent::KeyEvent(Keys key, Events::EventType type)
	: Event(type)
{
	m_Key = key;
}

/*   Purpose:  Destructor
 *       Pre:  None
 *      Post:  None
 ****************************************************************/
KeyEvent::~KeyEvent()
{
	dispose();
}

/*   Purpose:  This will clean up any memory for the KeyEvent
 *       Pre:  None
 *      Post:  None
 ****************************************************************/
void KeyEvent::dispose()
{

}