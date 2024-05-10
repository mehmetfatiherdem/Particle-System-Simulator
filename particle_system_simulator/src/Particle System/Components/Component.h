#pragma once

#include <stdint.h>
#include "Particle System/Data/ComponentType.h"

class ParticleSystemProps;
class Particle;

class Component
{
private:
	uint32_t priority;
	bool enabled;

protected:
	virtual void update(const ParticleSystemProps& props, Particle& particle) = 0;

public:
	Component() = delete;
	Component(uint32_t priority) : priority(priority), enabled(true) { }
	virtual ~Component() = default;

	void tryUpdate(const ParticleSystemProps& props, Particle& particle)
	{
		if (!enabled) return;
		update(props, particle);
	}

	virtual ComponentType getType() const = 0;
	uint32_t getPriority() const { return priority; }

	bool isEnabled() const { return enabled; }
	void enable() { enabled = true; }
	void disable() { enabled = false; }
	void toggle() { enabled = !enabled; }
};