#ifndef __PERFTIMER_H__
#define __PERFTIMER_H__

#include "Defs.h"

class PerfTimer
{
public:

	// Constructor
	PerfTimer();

	void Start();
	double ReadMs() const;
	uint64 ReadTicks() const;
	float ReadSec() const;

	float ReadSeconds();

	void ResetSeconds();

private:
	uint32	started_at;
	float offset = 1.0f;
	uint64 startTime;
	static uint64 frequency;

private:
	int counter = 0;
	int seconds = 0;
};

#endif //__PERFTIMER_H__