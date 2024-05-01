#include "GeneralUtility/Random.h"
#include "GeneralUtility/BasicMath.h"
#include "Particle System/Particle.h"
#include "Particle System/Data/ParticleSystemProps.h"
#include "Time Management/Time.h"
#include "RotationBySpeed.h"

void RotationBySpeed::update(const ParticleSystemProps& props, Particle& particle)
{
	float speed = glm::length(particle.velocity);
	float t = glm::clamp(speed, minSpeed, maxSpeed) / (maxSpeed - minSpeed);

	glm::vec3 rotation;

	switch (method)
	{
	case ComponentMethod::Curve:
		rotation = minBezier.evaluate(t);
		break;
	case ComponentMethod::RandomBetweenTwoCurves:
		glm::vec3 min = minBezier.evaluate(t);
		glm::vec3 max = maxBezier.evaluate(t);
		utility::math::swapToPreserveMinMax(min.x, max.x);
		utility::math::swapToPreserveMinMax(min.y, max.y);
		utility::math::swapToPreserveMinMax(min.z, max.z);
		rotation.x = Random::getFloat(min.x, max.x);
		rotation.y = Random::getFloat(min.y, max.y);
		rotation.z = Random::getFloat(min.z, max.z);
		break;
	}

	particle.renderer->getTransform().setEulerRotation(rotation);
}