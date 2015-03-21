/*
*	Vincent Loignon
*	Assignment 3
*	EventDispatcher.cpp
*	I certify that this assignment is entirely our own work.
*/

#include "EventDispatcher.h"
#include "Event.h"
#include "Timer.h"

using namespace Events;

// Global EventDispatcher Variable
EventDispatcher *g_EventDispatcher = NULL;

/*   Purpose:  Constructor
 *       Pre:  None
 *      Post:  None
 ****************************************************************/
EventDispatcher::EventDispatcher()
{
	m_Timer = NULL;
}

/*   Purpose:  Constructor
 *       Pre:  Timer*
 *      Post:  None
 ****************************************************************/
EventDispatcher::EventDispatcher(Timer* timer)
{
	m_Timer = timer; // Stores a reference to the game's master timer for use with delayed events
}

/*   Purpose:  Destructor
 *       Pre:  None
 *      Post:  None
 ****************************************************************/
EventDispatcher::~EventDispatcher()
{
	
}

/*   Purpose:  This will clean up any memory associated with the Event System
*       Pre:  None
*      Post:  None
****************************************************************/
void EventDispatcher::dispose()
{
	std::multimap<EventType, Delegate*>::iterator it;

	for (it = m_Events.begin(); it != m_Events.end(); it++)
	{
		delete it->second;
		it->second = NULL;
	}

	m_Events.clear();

	list<Event*>::iterator iter = m_EventQueue.begin();

	while (iter != m_EventQueue.end())
	{
		if ((*iter) != NULL)
		{
			(*iter)->dispose();
			delete (*iter);
			(*iter) = NULL;

			iter++;
		}
	}

	m_EventQueue.clear();
}

/*   Purpose:  This will process all the delayed events in the queue and fire them when they are ready
 *       Pre:  None
 *      Post:  None
 ****************************************************************/
void EventDispatcher::processEvents()
{
	double currentTime = m_Timer->getElapsedTime();

	list<Event*>::iterator iter = m_EventQueue.begin();
	while (iter != m_EventQueue.end())
	{
		if ((*iter)->getScheduledTime() <= currentTime)
		{
			dispatch((*iter));
			iter = m_EventQueue.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

/*   Purpose:  This will dispatch the event passed to all listeners for it
 *       Pre:  Event* ev
 *      Post:  None
 ****************************************************************/
void EventDispatcher::dispatch(Event* ev)
{
	std::pair<std::multimap<EventType, Delegate*>::iterator, std::multimap<EventType, Delegate*>::iterator> ret;
	ret = m_Events.equal_range(ev->getType());

	std::multimap<EventType, Delegate*>::iterator it;

	for (it = ret.first; it != ret.second; it++)
	{
		it->second->Invoke(ev);
	}

	ev->dispose();
	delete ev;
	ev = NULL;
}

/*   Purpose:  This will dispatch the event passed to all listeners for it
 *       Pre:  Event* ev
 *      Post:  None
 ****************************************************************/
void EventDispatcher::dispatchEvent(Event* ev)
{
	if (m_Timer != NULL)
	{
		double currentTime = m_Timer->getElapsedTime();

		//set frame numbers
		ev->m_SubmittedTime = currentTime;
		ev->m_ScheduledTime = currentTime;
	}
	else
	{
		ev->m_SubmittedTime = 0;
		ev->m_ScheduledTime = 0;
	}

	dispatch(ev);
}

/*   Purpose:  This will add the event to the queue with the delay specified (in milliseconds)
 *       Pre:  Event* ev, int delay
 *      Post:  None
 ****************************************************************/
void EventDispatcher::dispatchEvent(Event* ev, int delay)
{
	if (m_Timer == NULL)
		throw nullptr;

	double currentTime = m_Timer->getElapsedTime();

	//set frame numbers
	ev->m_SubmittedTime = currentTime;
	ev->m_ScheduledTime = currentTime + delay;

	m_EventQueue.push_back(ev);
}

/*   Purpose:  This will add an Event Listener to the multimap for the event system
 *       Pre:  EventType type, Delegate* handler
 *      Post:  None
 ****************************************************************/
void EventDispatcher::addEventListener(EventType type, Delegate* handler)
{
	m_Events.insert(std::pair<EventType, Delegate*>(type, handler));
}

/*   Purpose:  This will remove a specific Event Listener from the event system
 *       Pre:  EventType type, Delegate* handler, bool cleanUpDelegate
 *      Post:  None
 ****************************************************************/
void EventDispatcher::removeEventListener(EventType type, Delegate* handler, bool cleanUpDelegate /* = true */)
{
	std::pair<std::multimap<EventType, Delegate*>::iterator, std::multimap<EventType, Delegate*>::iterator> ret;
	ret = m_Events.equal_range(type);

	std::multimap<EventType, Delegate*>::iterator it;

	for (it = ret.first; it != ret.second; it++)
	{
		if (it->second == handler)
		{
			if (cleanUpDelegate)
			{
				delete it->second;
				it->second = NULL;
			}

			m_Events.erase(it);
			break;
		}
	}
}

/*   Purpose:  This will remove all Event Listeners of a specific type
 *       Pre:  EventType type, bool cleanUpDelegate
 *      Post:  None
 ****************************************************************/
void EventDispatcher::removeEventListeners(EventType type, bool cleanUpDelegate /* = true */)
{
	std::pair<std::multimap<EventType, Delegate*>::iterator, std::multimap<EventType, Delegate*>::iterator> ret;
	ret = m_Events.equal_range(type);

	std::multimap<EventType, Delegate*>::iterator it;

	if (cleanUpDelegate)
	{
		for (it = ret.first; it != ret.second; it++)
		{
			delete it->second;
			it->second = NULL;
		}
	}

	m_Events.erase(type);
}

/*   Purpose:  This will remove all Event Listeners attached to the specific Delegate
 *       Pre:  Delegate* handler, bool cleanUpDelegate
 *      Post:  None
 ****************************************************************/
void EventDispatcher::removeEventListenerAll(Delegate* handler, bool cleanUpDelegate /* = true */)
{
	std::multimap<EventType, Delegate*>::iterator it;
	
	for (it = m_Events.begin(); it != m_Events.end(); it++)
	{
		if (it->second == handler)
		{
			if (cleanUpDelegate)
			{
				delete it->second;
				it->second = NULL;
			}

			m_Events.erase(it);
		}
	}
}