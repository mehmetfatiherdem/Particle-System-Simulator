#pragma once

#include "GeneralUtility/Random.h"
#include "GeneralUtility/BasicMath.h"
#include "Particle System/Particle.h"
#include "Particle System/Data/ParticleSystemProps.h"
#include "Time Management/Time.h"
#include "VelocityOverLifetime.h"

void VelocityOverLifetime::update(const ParticleSystemProps& props, Particle& particle)
{
	float t = 1.0f - particle.remainingLifetime / props.startLifetime;
	glm::vec3 velocity = glm::vec3{0.0f, 0.0f, 0.0f};

	switch (method)
	{
	case ComponentMethod::Constant:
		velocity = minVelocity;
		break;
	case ComponentMethod::Random_Between_Two_Constants:
		velocity.x = Random::getFloat(minVelocity.x, maxVelocity.x);
		velocity.y = Random::getFloat(minVelocity.y, maxVelocity.y);
		velocity.z = Random::getFloat(minVelocity.z, maxVelocity.z);
		break;
	case ComponentMethod::Curve:
		velocity = minBezier.evaluate(t);
		break;
	case ComponentMethod::Random_Between_Two_Curves:
		glm::vec3 min = minBezier.evaluate(t);
		glm::vec3 max = maxBezier.evaluate(t);

		utility::math::swapToPreserveMinMax(min.x, max.x);
		utility::math::swapToPreserveMinMax(min.y, max.y);
		utility::math::swapToPreserveMinMax(min.z, max.z);

		velocity.x = Random::getFloat(min.x, max.x);
		velocity.y = Random::getFloat(min.y, max.y);
		velocity.z = Random::getFloat(min.z, max.z);
		break;
	}

	particle.velocity = velocity;
}
