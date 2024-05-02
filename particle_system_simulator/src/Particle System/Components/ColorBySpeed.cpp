#pragma once

#include "GeneralUtility/BasicMath.h"
#include "Particle System/Particle.h"
#include "Particle System/Data/ParticleSystemProps.h"
#include "ColorBySpeed.h"

void ColorBySpeed::update(const ParticleSystemProps& props, Particle& particle)
{
	float speed = glm::length(particle.velocity);
	float t = glm::clamp(speed, minSpeed, maxSpeed) / (maxSpeed - minSpeed);
	Color4 color{glm::vec3{0.0f, 0.0f, 0.0f}};

	if (t > keypoints[0] && t < keypoints[1])
	{
		color.ambient = utility::math::lerp(colors[0].ambient, colors[1].ambient, t);
		color.diffuse = utility::math::lerp(colors[0].diffuse, colors[1].diffuse, t);
		color.specular = utility::math::lerp(colors[0].specular, colors[1].specular, t);
	}
	else if (t < keypoints[0])
	{
		color = colors[0];
	}
	else
	{
		color = colors[1];
	}

	particle.renderer->getMaterial().setColor(color);
}
