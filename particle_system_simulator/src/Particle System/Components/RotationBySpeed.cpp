#include "GeneralUtility/Random.h"
#include "GeneralUtility/BasicMath.h"
#include "Particle System/Particle.h"
#include "Particle System/Data/ParticleSystemProps.h"
#include "Time Management/Time.h"
#include "Persistence/Serializer.h"
#include "Persistence/Serialization Utils/SerializationUtils.h"
#include "RotationBySpeed.h"

void RotationBySpeed::update(const ParticleSystemProps& props, Particle& particle)
{
	float speed = glm::length(particle.velocity);
	float t = glm::clamp(speed, minSpeed, maxSpeed) / (maxSpeed - minSpeed);

	float rotation = 0.0f;

	switch (method)
	{
	case ComponentMethod::Curve:
		rotation = minBezier.evaluatePoint(t);
		break;
	case ComponentMethod::Random_Between_Two_Curves:
		float min = minBezier.evaluatePoint(t);
		float max = maxBezier.evaluatePoint(t);
		utility::math::swapToPreserveMinMax(min, max);
		rotation = Random::getFloat(min, max);
		break;
	}
	
	particle.rotation = rotation;
}

void RotationBySpeed::serialize(Serializer& serializer, const std::string& objectName) const
{
	Component::serialize(serializer, objectName);
	serializer["ComponentMethod"].string(getComponentMethodName(method).c_str());

	serializer["MinSpeed"].real(minSpeed);
	serializer["MaxSpeed"].real(maxSpeed);

	persistence::utils::serializeBezier(serializer, minBezier, "MinBezier");
	persistence::utils::serializeBezier(serializer, maxBezier, "MaxBezier");

	serializer.endObject();
}