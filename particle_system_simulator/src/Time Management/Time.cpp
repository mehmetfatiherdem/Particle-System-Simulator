#include "GeneralUtility/gl2fw3.h"
#include "Time.h"

double castTime(double time, bool milliseconds = false)
{
	switch(milliseconds)
	{
	case false:
		return time;
	case true:
		return time * 1000;
	}
}

double Time::startTime = 0;
double Time::lastFrame = 0;
double Time::_deltaTime = 0;
double Time::_maxDeltaTime = 100000000;
double Time::_avgDeltaTime = 0;
unsigned long long Time::_frameCount = 0;

void Time::start()
{
	startTime = glfwGetTime();
	lastFrame = startTime;
}

void Time::endFrame()
{
	float currentFrame = glfwGetTime();
	_deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	_avgDeltaTime = (currentFrame - startTime) / ++_frameCount;

	if(_deltaTime < _maxDeltaTime)
	{
		_maxDeltaTime = _deltaTime;
	}
}

float Time::time(bool milliseconds)
{
	return castTime(glfwGetTime() - startTime, milliseconds);
}

float Time::deltaTime(bool milliseconds)
{
	return castTime(_deltaTime, milliseconds);
}

float Time::maxDeltaTime(bool milliseconds)
{
	return castTime(_maxDeltaTime, milliseconds);
}

float Time::avgDeltaTime(bool milliseconds)
{
	return castTime(_avgDeltaTime, milliseconds);
}

float Time::fps()
{
	return 1.0 / castTime(_deltaTime);
}

float Time::avgFps()
{
	return 1.0 / castTime(_avgDeltaTime);
}

float Time::maxFps()
{
	return 1.0 / castTime(_maxDeltaTime);
}

unsigned long long Time::frameCount()
{
	return _frameCount;
}
