#pragma once

#include <glm/vec3.hpp>
#include "Particle System/Data/ComponentMethod.h"
#include "GeneralUtility/BezierCurve.h"
#include "Component.h"

class ParticleSystemEditor;

class VelocityOverLifetime : public Component
{
private:
	friend class ParticleSystemEditor;

	ComponentMethod method = ComponentMethod::Constant;
	glm::vec3 minVelocity = glm::vec3{0.0f, 0.0f, 0.0f};
	glm::vec3 maxVelocity = glm::vec3{1.0f, 1.0f, 1.0f};
	BezierCurve<glm::vec3> minBezier = Bezier::createLinearVec3();
	BezierCurve<glm::vec3> maxBezier = Bezier::createLinearVec3();

protected:
	virtual void update(const ParticleSystemProps& props, Particle& particle) override;

public:
	VelocityOverLifetime() : Component(0) { }

	VelocityOverLifetime(const glm::vec3& constantVelocity) :
		Component(0), method(ComponentMethod::Constant), minVelocity(constantVelocity), maxVelocity(constantVelocity)
	{}
	VelocityOverLifetime(const glm::vec3& minVelocity, const glm::vec3& maxVelocity) :
		Component(0), method(ComponentMethod::Random_Between_Two_Constants), minVelocity(minVelocity), maxVelocity(maxVelocity)
	{}
	VelocityOverLifetime(const BezierCurve<glm::vec3>& bezier) :
		Component(0), method(ComponentMethod::Curve), minBezier(bezier), maxBezier(bezier)
	{}
	VelocityOverLifetime(const BezierCurve<glm::vec3>& minBezier, const BezierCurve<glm::vec3>& maxBezier) :
		Component(0), method(ComponentMethod::Random_Between_Two_Curves), minBezier(minBezier), maxBezier(maxBezier)
	{}

	VelocityOverLifetime(ComponentMethod method, const glm::vec3& minVelocity, const glm::vec3& maxVelocity,
		const BezierCurve<glm::vec3>& minBezier, const BezierCurve<glm::vec3>& maxBezier) : Component(0), method(method),
		minVelocity(minVelocity), maxVelocity(maxVelocity), minBezier(minBezier), maxBezier(maxBezier) { }

	virtual ~VelocityOverLifetime() override = default;

	virtual ComponentType getType() const override { return ComponentType::Velocity_Over_Lifetime; }

	virtual void serialize(Serializer& serializer, const std::string& objectName = "") const override;
};