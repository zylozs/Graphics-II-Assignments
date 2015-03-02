/*
*	Vincent Loignon
*	Assignment 3
*	IObject.h
*	I certify that this assignment is entirely our own work.
*/

#ifndef IObject_H
#define IObject_H

#include "Trackable.h"

// This interface is meant to be inherited by classes that want to have a dispose function.
// In most cases, it is good to inherit from IObject, but not necessarily in all cases.

class IObject : public Trackable
{
public:
	virtual void dispose() = 0;
};

#endif