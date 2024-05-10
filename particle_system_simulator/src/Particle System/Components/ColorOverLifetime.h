#pragma once

#include "Component.h"
#include "RenderPipeline/Color/Color4.h"

class ParticleSystemEditor;

class ColorOverLifetime : public Component
{
private:
	friend class ParticleSystemEditor;

	float keypoints[2] = {0.0f, 1.0f};
	Color4 colors[2] = {Color4{glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}}, Color4{glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}}};

protected:
	virtual void update(const ParticleSystemProps& props, Particle& particle) override;

public:

	ColorOverLifetime() : Component(3) { }

	ColorOverLifetime(float keypoint1, const Color4& color1, float keypoint2, const Color4& color2) :
		Component(3), keypoints{keypoint1, keypoint2}, colors{color1, color2} { }

	virtual ~ColorOverLifetime() override = default;

	virtual ComponentType getType() const override { return ComponentType::Color_Over_Lifetime; }
};

