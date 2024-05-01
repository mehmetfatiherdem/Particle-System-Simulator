#pragma once

#include "GeneralUtility/Random.h"
#include "GeneralUtility/BasicMath.h"
#include "Particle System/Particle.h"
#include "Particle System/Data/ParticleSystemProps.h"
#include "Time Management/Time.h"
#include "LimitVelocityOverLifetime.h"

void LimitVelocityOverLifetime::update(const ParticleSystemProps& props, Particle& particle)
{
	float t = 1.0f - particle.remainingLifetime / props.startLifetime;
	glm::vec3 limitVelocity = glm::vec3{0.0f, 0.0f, 0.0f};

	switch (method)
	{
	case ComponentMethod::Constant:
		limitVelocity = minVelocity;
		break;
	case ComponentMethod::RandomBetweenTwoConstants:
		limitVelocity.x = Random::getFloat(minVelocity.x, maxVelocity.x);
		limitVelocity.y = Random::getFloat(minVelocity.y, maxVelocity.y);
		limitVelocity.z = Random::getFloat(minVelocity.z, maxVelocity.z);
		break;
	case ComponentMethod::Curve:
		limitVelocity = minBezier.evaluate(t);
		break;
	case ComponentMethod::RandomBetweenTwoCurves:
		glm::vec3 min = minBezier.evaluate(t);
		glm::vec3 max = maxBezier.evaluate(t);

		utility::math::swapToPreserveMinMax(min.x, max.x);
		utility::math::swapToPreserveMinMax(min.y, max.y);
		utility::math::swapToPreserveMinMax(min.z, max.z);

		limitVelocity.x = Random::getFloat(min.x, max.x);
		limitVelocity.y = Random::getFloat(min.y, max.y);
		limitVelocity.z = Random::getFloat(min.z, max.z);
		break;
	}

	if (glm::length(particle.velocity) > glm::length(limitVelocity))
	{
		particle.velocity -= ((particle.velocity - limitVelocity) * dampen * Time::deltaTime());
	}
}
