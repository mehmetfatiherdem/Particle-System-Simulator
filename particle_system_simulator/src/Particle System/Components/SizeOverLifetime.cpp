#include "GeneralUtility/Random.h"
#include "GeneralUtility/BasicMath.h"
#include "Particle System/Particle.h"
#include "Particle System/Data/ParticleSystemProps.h"
#include "SizeOverLifetime.h"

void SizeOverLifetime::update(const ParticleSystemProps& props, Particle& particle)
{
	float t = 1.0f - particle.remainingLifetime / props.startLifetime;
	float size = 0.0f;

	switch (method)
	{
	case ComponentMethod::Random_Between_Two_Constants:
		size = Random::getFloat(minSize, maxSize);
		break;
	case ComponentMethod::Curve:
		size = minBezier.evaluate(t);
		break;
	case ComponentMethod::Random_Between_Two_Curves:
		float min = minBezier.evaluate(t);
		float max = maxBezier.evaluate(t);
		utility::math::swapToPreserveMinMax(min, max);
		size = Random::getFloat(min, max);
		break;
	}

	size = glm::clamp(size, props.minSize, props.maxSize);
	particle.renderer->getTransform().setScale(glm::vec3{size, size, size});
}