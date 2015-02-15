/* Author:      Vincent Loignon
   Class:       CSI 310_01
   Assignment:  Midterm Assignment
   Assigned:    2/07/14
   Due:         3/10/14

Certification of Authenticity:
I certify that this assignment is entirely my own work.
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