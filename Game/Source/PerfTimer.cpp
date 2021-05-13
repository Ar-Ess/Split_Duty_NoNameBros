// ----------------------------------------------------
// Slow timer with microsecond precision
// ----------------------------------------------------

#include "PerfTimer.h"
#include "SDL\include\SDL_timer.h"

#include "Log.h"

uint64 PerfTimer::frequency = 0;

// L07: DONE 2: Fill Constructor, Start(),ReadMs() and ReadTicks() methods
// they are simple, one line each!

PerfTimer::PerfTimer()
{
	if (frequency == 0) frequency = SDL_GetPerformanceFrequency();

	Start();
}

void PerfTimer::Start()
{
	startTime = SDL_GetPerformanceCounter();
}

double PerfTimer::ReadMs() const
{
	return 1000.0 * (double(SDL_GetPerformanceCounter() - startTime) / double(frequency));
}

uint64 PerfTimer::ReadTicks() const
{
	return SDL_GetPerformanceCounter() - startTime;
}

float PerfTimer::ReadSec() const
{
	
	return float(SDL_GetTicks() - started_at) / 1000.0f * offset;;
}

float PerfTimer::ReadSeconds() 
{
	counter++;
	if (counter % 60 == 0)
	{
		seconds++;

		LOG("Seconds since start: %d", seconds);
	}
	return seconds;
}

void PerfTimer::ResetSeconds()
{
	counter = 0;
	seconds = 0;
}
