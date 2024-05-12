#pragma once

#include <stdint.h>
#include "Particle System/Data/ParticleSystemProps.h"
#include "Particle System/Particle.h"
#include "Particle System/Data/EmitterType.h"

class Emitter
{
private:
	friend class ParticleSystemEditor;

	float surplus;
protected:

	float emissionRate;

	virtual void emit(const ParticleSystemProps& props, glm::vec3& position, glm::vec3& velocity) = 0;

public:
	Emitter(float emissionRate = 10.0f) : emissionRate(emissionRate), surplus(0) {}
	virtual ~Emitter() = 0;

	virtual EmitterType getType() const = 0;
	void tryEmit(ParticleSystemProps& props, std::vector<Particle>& particlePool, uint32_t& poolIndex);
};