#pragma once

#include <glm/vec3.hpp>
#include "Particle System/Data/ComponentMethod.h"
#include "GeneralUtility/BezierCurve.h"
#include "Component.h"

class ParticleSystemEditor;

class ForceOverLifetime : public Component
{
private:
	friend class ParticleSystemEditor;

	ComponentMethod method = ComponentMethod::Constant;
	glm::vec3 minForce = glm::vec3{0.0f, 0.0f, 0.0f};
	glm::vec3 maxForce = glm::vec3{1.0f, 1.0f, 1.0f};
	BezierCurve<glm::vec3> minBezier = Bezier::createLinearVec3();
	BezierCurve<glm::vec3> maxBezier = Bezier::createLinearVec3();

protected:
	virtual void update(const ParticleSystemProps& props, Particle& particle) override;

public:
	ForceOverLifetime() : Component(2) { }

	ForceOverLifetime(const glm::vec3& constantForce) : 
		Component(2), method(ComponentMethod::Constant), minForce(constantForce), maxForce(constantForce) { }
	ForceOverLifetime(const glm::vec3& minForce, const glm::vec3& maxForce) :
		Component(2), method(ComponentMethod::Random_Between_Two_Constants), minForce(minForce), maxForce(maxForce) { }
	ForceOverLifetime(const BezierCurve<glm::vec3>& bezier) :
		Component(2), method(ComponentMethod::Curve), minBezier(bezier), maxBezier(bezier) { }
	ForceOverLifetime(const BezierCurve<glm::vec3>& minBezier, const BezierCurve<glm::vec3>& maxBezier) :
		Component(2), method(ComponentMethod::Random_Between_Two_Curves), minBezier(minBezier), maxBezier(maxBezier) { }

	virtual ~ForceOverLifetime() override = default;

	virtual ComponentType getType() const override { return ComponentType::Force_Over_Lifetime; }
};