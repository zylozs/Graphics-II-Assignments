/* Author:      Vincent Loignon
   Class:       CSI 310_01
   Assignment:  Midterm Assignment
   Assigned:    2/07/14
   Due:         3/10/14

Certification of Authenticity:
I certify that this assignment is entirely my own work.
*/

#ifndef EVENT_H
#define EVENT_H

#include "IObject.h"
#include "EventType.h"

class EventDispatcher;

class Event : public IObject
{
protected:
	Events::EventType m_Type;
	double m_SubmittedTime;
	double m_ScheduledTime;

public:
	Event(Events::EventType type); // Constructor
	~Event(); // Destructor

	// Accessors
	Events::EventType getType() const { return m_Type; }
	double getSubmittedTime() const { return m_SubmittedTime; };
	double getScheduledTime() const { return m_ScheduledTime; };

	virtual void dispose();

	friend class EventDispatcher;
};

#endif