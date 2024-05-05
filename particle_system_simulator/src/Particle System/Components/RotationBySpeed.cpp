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

	float rotation = 0.0f;

	switch (method)
	{
	case ComponentMethod::Curve:
		rotation = minBezier.evaluate(t);
		break;
	case ComponentMethod::RandomBetweenTwoCurves:
		float min = minBezier.evaluate(t);
		float max = maxBezier.evaluate(t);
		utility::math::swapToPreserveMinMax(min, max);
		rotation = Random::getFloat(min, max);
		break;
	}
	
	particle.rotation = rotation;
}
