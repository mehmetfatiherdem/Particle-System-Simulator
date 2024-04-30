#pragma once

#include <glm/glm.hpp>
#include "Emitter.h"

class ConeEmitter : public Emitter
{
private:
	float radius;
	float angle;

protected:
	void emit(const ParticleSystemProps& props, glm::vec3& position, glm::vec3& velocity) override;

public:
	ConeEmitter(float emissionRate, float radius = 1.0f, float angle = glm::radians(50.0f)) : 
		Emitter(emissionRate), radius(radius), angle(angle) { }

};