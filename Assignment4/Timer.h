/*
*	Vincent Loignon
*	Assignment 4
*	Timer.h
*	I certify that this assignment is entirely our own work.
*/

#pragma once
#include <windows.h>
#include "Trackable.h"

class Timer : public Trackable
{
public:
	Timer();
	~Timer();

	void start();
	void stop();
	double getElapsedTime() const;//returns how much time has elapsed since start
	void sleepUntilElapsed( double ms );
	void pause( bool shouldPause );
	inline double getFactor() const { return mFactor; };
	inline void multFactor( double mult ) { mLastFactor = mFactor; mFactor *= mult; };
	inline void setFactor( double theFactor ) { mLastFactor = mFactor; mFactor = theFactor; };
	inline void restoreLastFactor() { mFactor = mLastFactor; };

private:
	LARGE_INTEGER mStartTime;
	LARGE_INTEGER mEndTime;
	LARGE_INTEGER mTimerFrequency;
	double mElapsedTime;
	double mFactor;
	double mLastFactor;
	bool mPaused;

	//helper function - uses current frequency for the Timer
	double calcDifferenceInMS( LARGE_INTEGER from, LARGE_INTEGER to ) const;

};