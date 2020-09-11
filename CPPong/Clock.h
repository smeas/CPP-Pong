#pragma once
#include <SDL2/SDL.h>

class Clock
{
	Uint64 mLastTime;
	Uint64 mCurrentTime;
	const Uint64 mFrequency;

public:
	Clock() : mFrequency(SDL_GetPerformanceFrequency())
	{
		mLastTime = SDL_GetPerformanceCounter();
		mCurrentTime = SDL_GetPerformanceCounter();
	}
	
	void Tick()
	{
		mLastTime = mCurrentTime;
		mCurrentTime = SDL_GetPerformanceCounter();
	}

	float Time() const
	{
		return (float)((double)mCurrentTime / (double)mFrequency);
	}
	
	float DeltaTime() const
	{
		return (float)((double)(mCurrentTime - mLastTime) / (double)mFrequency);
	}
};

