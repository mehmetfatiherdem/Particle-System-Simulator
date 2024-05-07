#pragma once

#include "Component.h"
#include "RenderPipeline/Color/Color4.h"

class ParticleSystemEditor;

class ColorBySpeed : public Component
{
private:
	friend class ParticleSystemEditor;

	float keypoints[2];
	Color4 colors[2];
	float minSpeed;
	float maxSpeed;

public:
	ColorBySpeed() = delete;

	ColorBySpeed(float keypoint1, const Color4& color1, float keypoint2, const Color4& color2, float minSpeed, float maxSpeed) :
		Component(6), keypoints{keypoint1, keypoint2}, colors{color1, color2}, minSpeed(minSpeed), maxSpeed(maxSpeed) { }

	virtual ~ColorBySpeed() override = default;

	virtual void update(const ParticleSystemProps& props, Particle& particle) override;

	virtual ComponentType getType() const override { return ComponentType::Color_By_Speed; }
};