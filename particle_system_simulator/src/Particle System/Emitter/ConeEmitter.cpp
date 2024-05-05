#include <cmath>
#include "ConeEmitter.h"
#include "GeneralUtility/MathConstants.h"
#include "GeneralUtility/Random.h"
#include "GeneralUtility/SignUtility.h"

float findBiasedDirection(float bias, float direction)
{
	float biasedMin, biasedMax;

	if (bias < 0)
	{
		biasedMax = -direction * bias;
		biasedMin = -direction;
	}
	else
	{
		biasedMin = -direction * bias;
		biasedMax = direction;
	}

	return Random::getFloat(biasedMin, biasedMax);
}

void ConeEmitter::emit(const ParticleSystemProps& props, glm::vec3& position, glm::vec3& velocity)
{
	float halfAngle = angle / 2;
	float theta = Random::getFloat(0.0f, TAU);

	float x = radius * std::cos(theta);
	float z = radius * std::sin(theta);
	float y = radius * std::tan(halfAngle) + Random::getFloat(-radius / 5.0f, radius / 5.0f);

	float dirX = findBiasedDirection(x / radius, halfAngle);
	float dirZ = findBiasedDirection(z / radius, halfAngle);
	float dirY = std::sqrtf(1.0f - dirX * dirX - dirZ * dirZ);

	position = glm::vec3{x, y, z} + props.position;
	velocity = glm::vec3{dirX, dirY, dirZ} * props.startSpeed;
}