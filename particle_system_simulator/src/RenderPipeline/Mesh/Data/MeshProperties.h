#pragma once

struct MeshProperties
{
	bool instanced;
	unsigned int chunkSize = 25;
	unsigned int estimatedNrOfInstances = 0;

	explicit MeshProperties() : instanced(false), chunkSize(0), estimatedNrOfInstances(0) { }
	explicit MeshProperties(bool instanced, unsigned int estimatedNrOfInstances = 0, unsigned int chunkSize = 25) : instanced(instanced),
		chunkSize(chunkSize ? chunkSize : 25), estimatedNrOfInstances(instanced ? (estimatedNrOfInstances ? estimatedNrOfInstances : this->chunkSize) : 0) { }
};