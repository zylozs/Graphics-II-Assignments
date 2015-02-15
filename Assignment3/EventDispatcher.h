/* Author:      Vincent Loignon
   Class:       CSI 310_01
   Assignment:  Midterm Assignment
   Assigned:    2/07/14
   Due:         3/10/14

Certification of Authenticity:
I certify that this assignment is entirely my own work.
*/

#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H

#include <map>
#include <list>
#include "Delegate.h"
#include "IObject.h"
#include "EventType.h"

class Event;
class Timer;

class EventDispatcher : public IObject
{
protected:
	std::multimap<Events::EventType, Delegate*> m_Events;
	std::list<Event*> m_EventQueue;

	Timer* m_Timer; // Reference to the game's "master timer"

	void dispatch(Event* ev);

public:
	EventDispatcher(); // Constructor
	EventDispatcher(Timer* timer); // Constructor
	~EventDispatcher(); // Destructor

	void registerTimer(Timer* timer) { m_Timer = timer; } // Stores a reference to the game's master timer for use with delayed events

	void processEvents();

	// Functionality
	void dispatchEvent(Event* ev);
	void dispatchEvent(Event* ev, int delay);
	void addEventListener(Events::EventType type, Delegate* handler);
	void removeEventListener(Events::EventType type, Delegate* handler, bool cleanUpDelegate = true);
	void removeEventListeners(Events::EventType type, bool cleanUpDelegate = true);
	void removeEventListenerAll(Delegate* handler, bool cleanUpDelegate = true);

	virtual void dispose();
};

extern EventDispatcher* g_EventDispatcher;
#endif