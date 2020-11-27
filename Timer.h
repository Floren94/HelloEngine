#pragma once
#include "SDL/include/SDL.h"

class Timer
{
public:
	Timer() {};
	~Timer() {};

	void StartTimer();
	void StartPerformanceTimer();
	void StopTimer();
	unsigned int GetTime();
	unsigned int GetPerformanceTime();

private:
	unsigned int lastTime, currentTime, startPerform, currentPerform;
};

inline
void Timer::StartTimer() {
	lastTime = currentTime = 0;
}

inline
void Timer::StartPerformanceTimer() {
	startPerform = 0;
}

inline
void Timer::StopTimer() {
	lastTime = currentTime = 0;
}

inline
unsigned int Timer::GetTime() {
	currentTime = SDL_GetTicks();
	return currentTime - lastTime;
}

inline
unsigned int Timer::GetPerformanceTime() {
	currentPerform = SDL_GetPerformanceCounter();
	int r = ((currentPerform - startPerform) * 1000) / SDL_GetPerformanceFrequency();
	startPerform = currentTime;
	return r;
}
