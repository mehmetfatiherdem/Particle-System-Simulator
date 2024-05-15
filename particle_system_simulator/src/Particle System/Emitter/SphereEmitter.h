#pragma once

#include <memory>
#include "Emitter.h"
#include "GeneralUtility/MathConstants.h"

class ParticleSystemProps;
class ParticleSystemEditor;

class SphereEmitter : public Emitter
{
private:
	friend class ParticleSystemEditor;
	float radius;
	float arc;

protected:
	virtual void emit(const ParticleSystemProps& props, glm::vec3& position, glm::vec3& velocity) override;

public:
	SphereEmitter(float emissionRate, float radius = 10.0f, float arc = TAU) : Emitter(emissionRate), radius(radius), arc(arc) { }
	virtual ~SphereEmitter() override = default;

	virtual EmitterType getType() const override { return EmitterType::SphereEmitter; }

	static std::unique_ptr<SphereEmitter> defaultEmitter();

	virtual void serialize(Serializer& serializer, const std::string& objectName = "") const override;
};