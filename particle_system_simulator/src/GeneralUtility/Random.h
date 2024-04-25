#pragma once

#include <random>
#include <stdint.h>

class Random
{
private:
	static std::mt19937 engine;
	static std::uniform_int_distribution<std::mt19937::result_type> distribution;

public:
	static void init()
	{
		engine.seed(std::random_device()());
	};

	static float getFloat()
	{
		return (float)distribution(engine) / (float)std::numeric_limits<uint32_t>::max();
	}

	static float getFloat(float min, float max)
	{
		float random = getFloat();
		return min + (max - min) * random;
	}

	static int getInt(int min, int max)
	{
		float random = getFloat();
		return min + (int)((max - min) * random);
	}
};