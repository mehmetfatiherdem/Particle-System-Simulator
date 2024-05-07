#pragma once

#include <stdint.h>
#include "Particle System/Data/ComponentType.h"

class ParticleSystemProps;
class Particle;

class Component
{
private:
	uint32_t priority;

public:
	Component() = delete;
	Component(uint32_t priority) : priority(priority) { }
	virtual ~Component() = default;

	virtual void update(const ParticleSystemProps& props, Particle& particle) = 0;
	virtual ComponentType getType() const = 0;

	uint32_t getPriority() const { return priority; }
};