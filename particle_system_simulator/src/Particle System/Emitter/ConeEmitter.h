#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "Emitter.h"

class ParticleSystemEditor;

class ConeEmitter : public Emitter
{
private:
	friend class ParticleSystemEditor;
	float radius;
	float angle;

protected:
	virtual void emit(const ParticleSystemProps& props, glm::vec3& position, glm::vec3& velocity) override;

public:
	ConeEmitter(float emissionRate, float radius = 1.0f, float angle = glm::radians(50.0f)) : 
		Emitter(emissionRate), radius(radius), angle(angle) { }

	virtual ~ConeEmitter() override = default;

	static std::unique_ptr<ConeEmitter> defaultEmitter();
};