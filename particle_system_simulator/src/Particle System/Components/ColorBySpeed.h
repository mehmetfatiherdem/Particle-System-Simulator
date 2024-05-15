#pragma once

#include "Component.h"
#include "RenderPipeline/Color/Color4.h"

class ParticleSystemEditor;

class ColorBySpeed : public Component
{
private:
	friend class ParticleSystemEditor;

	float keypoints[2] = {0.0f, 1.0f};
	Color4 colors[2] = {Color4{glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}}, Color4{glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}}};
	float minSpeed = 0.0f;
	float maxSpeed = 1.0f;

protected:
	virtual void update(const ParticleSystemProps& props, Particle& particle) override;

public:
	ColorBySpeed() : Component(6) { }

	ColorBySpeed(float keypoint1, const Color4& color1, float keypoint2, const Color4& color2, float minSpeed, float maxSpeed) :
		Component(6), keypoints{keypoint1, keypoint2}, colors{color1, color2}, minSpeed(minSpeed), maxSpeed(maxSpeed) { }

	virtual ~ColorBySpeed() override = default;

	virtual ComponentType getType() const override { return ComponentType::Color_By_Speed; }

	virtual void serialize(Serializer& serializer, const std::string& objectName = "") const override;
};