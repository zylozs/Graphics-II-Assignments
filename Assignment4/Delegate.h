/*
*	Vincent Loignon
*	Assignment 4
*	Delegate.h
*	I certify that this assignment is entirely our own work.
*/

#ifndef DELEGATE_H
#define DELEGATE_H

#include <iostream>
#include <string>
#include <cstring>
#include "Trackable.h"
#include "Event.h"

using namespace std;

class Delegate : public Trackable
{
	//Create some typedefs to manage references easier
	typedef void *InstancePtr;
	typedef void (*InternalFunction)(InstancePtr, Event*); //This holds an internal function reference

public:

	/*     Pre: A function and/or a reference to the IObject
	 *    Post: A delegate binded to function is returned
	 * Purpose: To create a delegate bound to a function
	 ***********************************************************/
	template <void (*Function)(Event*)>
	static Delegate* create(void)
    {
		Delegate* del = new Delegate();
		del->Bind<Function>();
        return del;
    }
	template <class C, void (C::*Function)(Event*)>
	static Delegate* create(C* obj)
    {
		Delegate* del = new Delegate();
		del->Bind<C, Function>(obj);
        return del;
    }

	/*     Pre: A function and/or a reference to the IObject
	 *    Post: A delegate binded to function is returned
	 * Purpose: To create a delegate bound to a function
	 ***********************************************************/
	template <void (*Function)(Event*)>
	void Bind(void)
	{
		mInst = NULL;
		mInternal = &Method<Function>;
	}
	template <class C, void (C::*Function)(Event*)>
	void Bind(C* obj)
	{
		mInst = obj;
		mInternal = &ClassMethod<C, Function>;
	}

	/*     Pre: This IObject exists and is bound to a function
	 *    Post: The function is fired
	 * Purpose: To create a delegate bound to a function
	 ***********************************************************/
	void Invoke(Event* ARG0)
	{
		return mInternal(mInst, ARG0);
	}

	/*     Pre: Functions, event data, or pointers
	 *    Post: The function is executed
	 * Purpose: To correctly fire a function based on whether
	 *			it's a void* function of a member function
	 ***********************************************************/
	template <void (*Function)(Event*)>
	static void Method(InstancePtr, Event* ARG0)
	{
		return (Function)(ARG0);
	}
	template <class C, void (C::*Function)(Event*)>
	static void ClassMethod(InstancePtr ptr, Event* ARG0)
	{
		return (static_cast<C*>(ptr)->*Function)(ARG0);
	}

private:

	Delegate(void) {}

	InternalFunction mInternal;
	InstancePtr mInst;

};

#endif