#pragma once

#include "Emitter.h"

#include <glm/vec2.hpp>
#include "GeneralUtility/Random.h"
#include "GeneralUtility/BasicMath.h"
#include "GeneralUtility/Trigonometry.h"
#include "Time Management/Time.h"

class ParticleSystemProps;

class SphereEmitter : public Emitter
{
private:
	float radius;
	float arc;

protected:
	void emit(const ParticleSystemProps& props, std::vector<Particle>& particlePool, uint32_t& poolIndex, uint32_t particleCount) override;

public:
	SphereEmitter(float emissionRate, float radius = 10.0f, float arc = TAU) : Emitter(emissionRate), radius(radius), arc(arc) {}
};