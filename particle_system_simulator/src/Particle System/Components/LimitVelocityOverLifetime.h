#include <glm/vec3.hpp>
#include "Particle System/Data/ComponentMethod.h"
#include "GeneralUtility/CubicBezierCurve.h"
#include "Component.h"

class ParticleSystemEditor;

class LimitVelocityOverLifetime : public Component
{
private:
	friend class ParticleSystemEditor;

	ComponentMethod method = ComponentMethod::Constant;
	float dampen = 0.5f;
	glm::vec3 minVelocity = glm::vec3{0.0f, 0.0f, 0.0f};
	glm::vec3 maxVelocity = glm::vec3{1.0f, 1.0f, 1.0f};
	CubicBezierCurve<glm::vec3> minBezier = CubicBezierCurve<glm::vec3>(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(2.0f), glm::vec3(3.0f));
	CubicBezierCurve<glm::vec3> maxBezier = CubicBezierCurve<glm::vec3>(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(2.0f), glm::vec3(3.0f));

protected:
	virtual void update(const ParticleSystemProps& props, Particle& particle) override;

public:
	LimitVelocityOverLifetime() : Component(7) { }

	LimitVelocityOverLifetime(const glm::vec3& constantVelocity, float dampen) :
		Component(7), method(ComponentMethod::Constant), minVelocity(constantVelocity), maxVelocity(constantVelocity), dampen(dampen)
	{}
	LimitVelocityOverLifetime(const glm::vec3& minVelocity, const glm::vec3& maxVelocity, float dampen) :
		Component(7), method(ComponentMethod::Random_Between_Two_Constants), minVelocity(minVelocity), maxVelocity(maxVelocity), dampen(dampen)
	{}
	LimitVelocityOverLifetime(const CubicBezierCurve<glm::vec3>& bezier, float dampen) :
		Component(7), method(ComponentMethod::Curve), minBezier(bezier), maxBezier(bezier), dampen(dampen)
	{}
	LimitVelocityOverLifetime(const CubicBezierCurve<glm::vec3>& minBezier, const CubicBezierCurve<glm::vec3>& maxBezier, float dampen) :
		Component(7), method(ComponentMethod::Random_Between_Two_Curves), minBezier(minBezier), maxBezier(maxBezier), dampen(dampen)
	{}

	virtual ~LimitVelocityOverLifetime() override = default;

	virtual ComponentType getType() const override { return ComponentType::Limit_Velocity_Over_Lifetime; }
};
