#pragma once

#include <stdint.h>

struct MeshProperties
{
	bool instanced;
	uint32_t chunkSize = 25;
	uint32_t estimatedNrOfInstances = 0;

	explicit MeshProperties() : instanced(false), chunkSize(0), estimatedNrOfInstances(0) {}
	explicit MeshProperties(bool instanced, uint32_t estimatedNrOfInstances = 0, uint32_t chunkSize = 25) : instanced(instanced),
		chunkSize(chunkSize ? chunkSize : 25), estimatedNrOfInstances(instanced ? (estimatedNrOfInstances ? estimatedNrOfInstances : this->chunkSize) : 0) { }
};