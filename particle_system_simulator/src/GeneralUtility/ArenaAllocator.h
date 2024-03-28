#pragma once

#include <stdint.h>
#include <set>

template <typename T>
class ArenaAllocator
{
private:
	uint8_t* buffer;
	size_t bufferSize;

	std::set<uint8_t*> holes;
};