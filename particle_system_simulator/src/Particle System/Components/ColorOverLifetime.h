#pragma once

#include "Component.h"
#include "RenderPipeline/Color/Color4.h"

class ColorOverLifetime : Component
{
private:
	float keypoints[2];
	Color4 colors[2];

public:

	ColorOverLifetime() = delete;

	ColorOverLifetime(float keypoint1, const Color4& color1, float keypoint2, const Color4& color2) :
		Component(0), keypoints{keypoint1, keypoint2}, colors{color1, color2} { }

	virtual ~ColorOverLifetime() override = default;

	virtual void update(const ParticleSystemProps& props, Particle& particle) override;
};

