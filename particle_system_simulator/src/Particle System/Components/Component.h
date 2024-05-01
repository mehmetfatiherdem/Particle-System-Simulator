#pragma once

class ParticleSystemProps;
class Particle;

class Component
{
public:
	virtual ~Component() = default;

	virtual void update(const ParticleSystemProps& props, Particle& particle) = 0;
};