#pragma once

#include <stdint.h>
#include "Particle System/Data/ParticleSystemProps.h"
#include "Particle System/Particle.h"

class Emitter
{
private:
	float emissionRate;
	float surplus;

protected:
	Emitter(float emissionRate = 10.0f) : emissionRate(emissionRate), surplus(0) {}

	virtual void emit(const ParticleSystemProps& props, std::vector<Particle>& particlePool, uint32_t& poolIndex, uint32_t particleCount) = 0;
public:

	void tryEmit(ParticleSystemProps& props, std::vector<Particle>& particlePool, uint32_t& poolIndex);
};