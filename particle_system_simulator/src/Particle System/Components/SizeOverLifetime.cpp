#include "GeneralUtility/Random.h"
#include "Particle System/Particle.h"
#include "Particle System/Data/ParticleSystemProps.h"
#include "SizeOverLifetime.h"

void SizeOverLifetime::update(const ParticleSystemProps& props, Particle& particle)
{
	float t = 1.0f - particle.remainingLifetime / props.startLifetime;
	float size = 0.0f;

	switch (method)
	{
	case ComponentMethod::RandomBetweenTwoConstants:
		size = Random::getFloat(minSize, maxSize);
		break;
	case ComponentMethod::Curve:
		size = minBezier.evaluate(t);
		break;
	case ComponentMethod::RandomBetweenTwoCurves:
		size = Random::getFloat(minBezier.evaluate(t), maxBezier.evaluate(t));
		break;
	}

	particle.renderer->getTransform().setScale(glm::vec3{size, size, size});
}