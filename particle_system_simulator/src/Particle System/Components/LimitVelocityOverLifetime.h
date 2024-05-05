#include <glm/vec3.hpp>
#include "Particle System/Data/ComponentMethod.h"
#include "GeneralUtility/CubicBezierCurve.h"
#include "Component.h"

class ParticleSystemEditor;

class LimitVelocityOverLifetime : public Component
{
private:
	friend class ParticleSystemEditor;

	ComponentMethod method;
	float dampen;
	glm::vec3 minVelocity;
	glm::vec3 maxVelocity;
	CubicBezierCurve<glm::vec3> minBezier;
	CubicBezierCurve<glm::vec3> maxBezier;

public:
	LimitVelocityOverLifetime() = delete;

	LimitVelocityOverLifetime(const glm::vec3& constantVelocity, float dampen) :
		Component(7), method(ComponentMethod::Constant), minVelocity(constantVelocity), maxVelocity(constantVelocity), dampen(dampen)
	{}
	LimitVelocityOverLifetime(const glm::vec3& minVelocity, const glm::vec3& maxVelocity, float dampen) :
		Component(7), method(ComponentMethod::RandomBetweenTwoConstants), minVelocity(minVelocity), maxVelocity(maxVelocity), dampen(dampen)
	{}
	LimitVelocityOverLifetime(const CubicBezierCurve<glm::vec3>& bezier, float dampen) :
		Component(7), method(ComponentMethod::Curve), minBezier(bezier), maxBezier(bezier), dampen(dampen)
	{}
	LimitVelocityOverLifetime(const CubicBezierCurve<glm::vec3>& minBezier, const CubicBezierCurve<glm::vec3>& maxBezier, float dampen) :
		Component(7), method(ComponentMethod::RandomBetweenTwoCurves), minBezier(minBezier), maxBezier(maxBezier), dampen(dampen)
	{}

	virtual ~LimitVelocityOverLifetime() override = default;

	virtual void update(const ParticleSystemProps& props, Particle& particle) override;
};
