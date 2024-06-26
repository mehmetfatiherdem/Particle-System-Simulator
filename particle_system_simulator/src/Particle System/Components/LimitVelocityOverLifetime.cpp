#pragma once

#include "GeneralUtility/Random.h"
#include "GeneralUtility/BasicMath.h"
#include "Particle System/Particle.h"
#include "Particle System/Data/ParticleSystemProps.h"
#include "Time Management/Time.h"
#include "Persistence/Serializer.h"
#include "Persistence/Serialization Utils/SerializationUtils.h"
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
	case ComponentMethod::Random_Between_Two_Constants:
		limitVelocity.x = Random::getFloat(minVelocity.x, maxVelocity.x);
		limitVelocity.y = Random::getFloat(minVelocity.y, maxVelocity.y);
		limitVelocity.z = Random::getFloat(minVelocity.z, maxVelocity.z);
		break;
	case ComponentMethod::Curve:
		limitVelocity = minBezier.evaluatePoint(t);
		break;
	case ComponentMethod::Random_Between_Two_Curves:
		glm::vec3 min = minBezier.evaluatePoint(t);
		glm::vec3 max = maxBezier.evaluatePoint(t);

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

void LimitVelocityOverLifetime::serialize(Serializer& serializer, const std::string& objectName) const
{
	Component::serialize(serializer, objectName);
	serializer["ComponentMethod"].string(getComponentMethodName(method).c_str());

	serializer["Dampen"].real(dampen);

	persistence::utils::serializeVector(serializer, minVelocity, "MinVelocity");
	persistence::utils::serializeVector(serializer, maxVelocity, "MaxVelocity");

	persistence::utils::serializeBezier(serializer, minBezier, "MinBezier");
	persistence::utils::serializeBezier(serializer, maxBezier, "MaxBezier");

	serializer.endObject();
}