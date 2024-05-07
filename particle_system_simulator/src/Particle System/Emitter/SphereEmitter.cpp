#include <glm/vec2.hpp>
#include "GeneralUtility/Random.h"
#include "GeneralUtility/BasicMath.h"
#include "GeneralUtility/Trigonometry.h"
#include "Time Management/Time.h"
#include "SphereEmitter.h"

void SphereEmitter::emit(const ParticleSystemProps& props, glm::vec3& position, glm::vec3& velocity)
{
	float dx, dy, x, y, z, randomRadius, randomXy, noise;

	//Maybe come up with a better way to generate random vectors
	do
	{
		dx = Random::getFloat(-1.0f, 1.0f);
		dy = Random::getFloat(-1.0f, 1.0f);
	} while (Trig::atan2(dy, dx) > arc);

	randomRadius = Random::getFloat(0.0f, radius);
	z = Random::getFloat(-randomRadius, randomRadius);
	randomXy = randomRadius * randomRadius - z * z;
	randomXy = std::sqrtf(randomXy / (dx * dx + dy * dy));
	y = randomXy * dy;
	x = randomXy * dx;
	noise = utility::math::max(x, y, z) / 5.0f;

	glm::vec3 offset = glm::vec3{x, y, z};
	velocity = offset + glm::vec3{Random::getFloat(-noise, noise), Random::getFloat(-noise, noise), Random::getFloat(-noise, noise)};
	velocity = glm::normalize(velocity) * props.startSpeed;
	position = offset + props.position;
}

std::unique_ptr<SphereEmitter> SphereEmitter::defaultEmitter()
{
	return std::make_unique<SphereEmitter>(10.0f);
}