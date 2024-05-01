#pragma once

#include "GeneralUtility/Random.h"
#include "Particle System/Particle.h"
#include "Particle System/Data/ParticleSystemProps.h"
#include "Time Management/Time.h"
#include "VelocityOverLifetime.h"

void VelocityOverLifetime::update(const ParticleSystemProps& props, Particle& particle)
{
	float t = 1.0f - particle.remainingLifetime / props.startLifetime;
	glm::vec3 force = glm::vec3{0.0f, 0.0f, 0.0f};

	switch (method)
	{
	case ComponentMethod::Constant:
		force = minVelocity;
		break;
	case ComponentMethod::RandomBetweenTwoConstants:
		force.x = Random::getFloat(minVelocity.x, maxVelocity.x);
		force.y = Random::getFloat(minVelocity.y, maxVelocity.y);
		force.z = Random::getFloat(minVelocity.z, maxVelocity.z);
		break;
	case ComponentMethod::Curve:
		force = minBezier.evaluate(t);
		break;
	case ComponentMethod::RandomBetweenTwoCurves:
		glm::vec3 min = minBezier.evaluate(t);
		glm::vec3 max = maxBezier.evaluate(t);
		force.x = Random::getFloat(min.x, max.x);
		force.y = Random::getFloat(min.y, max.y);
		force.z = Random::getFloat(min.z, max.z);
		break;
	}

	particle.velocity += force * Time::deltaTime();
}
