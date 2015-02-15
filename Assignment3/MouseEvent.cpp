/* Author:      Vincent Loignon
   Class:       CSI 310_01
   Assignment:  Midterm Assignment
   Assigned:    2/07/14
   Due:         3/10/14

Certification of Authenticity:
I certify that this assignment is entirely my own work.
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