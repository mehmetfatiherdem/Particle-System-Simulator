#include "GeneralUtility/Random.h"
#include "GeneralUtility/BasicMath.h"
#include "Particle System/Particle.h"
#include "Particle System/Data/ParticleSystemProps.h"
#include "Time Management/Time.h"
#include "Persistence/Serializer.h"
#include "Persistence/SerializationUtils.h"
#include "SizeBySpeed.h"

void SizeBySpeed::update(const ParticleSystemProps& props, Particle& particle)
{
	float speed = glm::length(particle.velocity);
	float t = glm::clamp(speed, minSpeed, maxSpeed) / (maxSpeed - minSpeed);

	float size = 0.0f;

	switch (method)
	{
	case ComponentMethod::Curve:
		size = minBezier.evaluatePoint(t);
		break;
	case ComponentMethod::Random_Between_Two_Curves:
		float min = minBezier.evaluatePoint(t);
		float max = maxBezier.evaluatePoint(t);
		utility::math::swapToPreserveMinMax(min, max);
		size = Random::getFloat(min, max);
		break;
	}

	size = glm::clamp(size, props.minSize, props.maxSize);
	particle.renderer->getTransform().setScale(glm::vec3{size, size, size});
}

void SizeBySpeed::serialize(Serializer& serializer, const std::string& objectName) const
{
	Component::serialize(serializer, objectName);
	serializer["ComponentMethod"].String(getComponentMethodName(method).c_str());

	serializer["MinSpeed"].Double(minSpeed);
	serializer["MaxSpeed"].Double(maxSpeed);

	persistence::utils::serializeBezierFloat(serializer, minBezier, "MinBezier");
	persistence::utils::serializeBezierFloat(serializer, maxBezier, "MaxBezier");

	serializer.endObject();
}