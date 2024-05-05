#pragma once

#include <random>
#include <stdint.h>

class Random
{
private:
	static std::mt19937 engine;
	static std::uniform_int_distribution<std::mt19937::result_type> distribution;

public:
	static void init();
	static float getFloat();
	static float getFloat(float min, float max);
	static int getInt(int min, int max);
};