/* Author:      Vincent Loignon
   Class:       CSI 310_01
   Assignment:  Midterm Assignment
   Assigned:    2/07/14
   Due:         3/10/14

Certification of Authenticity:
I certify that this assignment is entirely my own work.
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