/*
*	Vincent Loignon
*	Assignment 3
*	MemoryTracker.h
*	I certify that this assignment is entirely our own work.
*/

#ifndef MEMORY_TRACKER_H
#define MEMORY_TRACKER_H

#include <map>
#include <iostream>


struct AllocationRecord
{
	AllocationRecord( int theNum, size_t theSize, std::string theFile, int theLine ): num(theNum), size(theSize), file(theFile), line(theLine){};
	int num;
	size_t size;

	std::string file;
	int line;
};

class MemoryTracker
{
public:
	MemoryTracker(); // Default Constructor
	~MemoryTracker(); // Destructor

	void addAllocation( void* ptr, size_t size, std::string file, int line );
	void removeAllocation( void* ptr );
	
	void reportAllocations( std::ostream& stream );

private:
	//copying not allowed
	MemoryTracker( const MemoryTracker& );
	MemoryTracker& operator=( const MemoryTracker& );

	std::map<void*,AllocationRecord> mAllocations;

	static int msAllocationNum;
};

extern MemoryTracker g_MemoryTracker;

#endif