#include "Random.h"

std::mt19937 Random::engine{};
std::uniform_int_distribution<std::mt19937::result_type> Random::distribution{};

void Random::init()
{
	engine.seed(std::random_device()());
};

float Random::getFloat()
{
	return (float)distribution(engine) / (float)std::numeric_limits<uint32_t>::max();
}

float Random::getFloat(float min, float max)
{
	float random = getFloat();
	return min + (max - min) * random;
}

int Random::getInt(int min, int max)
{
	float random = getFloat();
	return min + (int)((max - min) * random);
}