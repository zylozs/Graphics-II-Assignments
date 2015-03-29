/*
*	Vincent Loignon
*	Assignment 4
*	Event.cpp
*	I certify that this assignment is entirely our own work.
*/
#include "Event.h"

/*   Purpose:  Constructor
 *       Pre:  EventType type
 *      Post:  None
 ****************************************************************/
Event::Event(Events::EventType type)
{
	m_Type = type;
}

/*   Purpose:  Destructor
 *       Pre:  None
 *      Post:  None
 ****************************************************************/
Event::~Event()
{
	dispose();
}

/*   Purpose:  This will clean up any memory for the Event
 *       Pre:  None
 *      Post:  None
 ****************************************************************/
void Event::dispose()
{

}