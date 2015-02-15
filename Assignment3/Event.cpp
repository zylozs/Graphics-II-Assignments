/* Author:      Vincent Loignon
   Class:       CSI 310_01
   Assignment:  Midterm Assignment
   Assigned:    2/07/14
   Due:         3/10/14

Certification of Authenticity:
I certify that this assignment is entirely my own work.
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