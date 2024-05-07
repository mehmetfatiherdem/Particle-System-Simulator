#pragma once

#include "GeneralUtility/Random.h"
#include "GeneralUtility/BasicMath.h"
#include "Particle System/Particle.h"
#include "Particle System/Data/ParticleSystemProps.h"
#include "Time Management/Time.h"
#include "ForceOverLifetime.h"

void ForceOverLifetime::update(const ParticleSystemProps& props, Particle& particle)
{
	float t = 1.0f - particle.remainingLifetime / props.startLifetime;
	glm::vec3 force = glm::vec3{0.0f, 0.0f, 0.0f};

	switch (method)
	{
	case ComponentMethod::Constant:
		force = minForce;
		break;
	case ComponentMethod::Random_Between_Two_Constants:
		force.x = Random::getFloat(minForce.x, maxForce.x);
		force.y = Random::getFloat(minForce.y, maxForce.y);
		force.z = Random::getFloat(minForce.z, maxForce.z);
		break;
	case ComponentMethod::Curve:
		force = minBezier.evaluate(t);
		break;
	case ComponentMethod::Random_Between_Two_Curves:
		glm::vec3 min = minBezier.evaluate(t);
		glm::vec3 max = maxBezier.evaluate(t);

		utility::math::swapToPreserveMinMax(min.x, max.x);
		utility::math::swapToPreserveMinMax(min.y, max.y);
		utility::math::swapToPreserveMinMax(min.z, max.z);

		force.x = Random::getFloat(min.x, max.x);
		force.y = Random::getFloat(min.y, max.y);
		force.z = Random::getFloat(min.z, max.z);
		break;
	}

	particle.velocity += force * Time::deltaTime();
}
