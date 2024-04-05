#pragma once

class Time
{
private:
	friend class Application;

	static double startTime;
	static double lastFrame;

	static double _deltaTime;
	static double _maxDeltaTime;
	static double _avgDeltaTime;

	static unsigned long long _frameCount;

	static void start();
	static void endFrame();
public:
	Time() = delete;

	static float time(bool milliseconds = false);
	static float deltaTime(bool milliseconds = false);
	static float maxDeltaTime(bool milliseconds = false);
	static float avgDeltaTime(bool milliseconds = false);
	static float fps();
	static float avgFps();
	static float maxFps();
	static unsigned long long frameCount();
};