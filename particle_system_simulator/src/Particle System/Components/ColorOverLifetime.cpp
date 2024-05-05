#pragma once

#include <iostream>
#include "GeneralUtility/BasicMath.h"
#include "Particle System/Particle.h"
#include "Particle System/Data/ParticleSystemProps.h"
#include "ColorOverLifetime.h"

void ColorOverLifetime::update(const ParticleSystemProps& props, Particle& particle)
{
	float t = 1.0f - particle.remainingLifetime / props.startLifetime;
	Color4 color;

	if (t > keypoints[0] && t < keypoints[1])
	{
		color.ambient = utility::math::lerp(colors[0].ambient, colors[1].ambient, t);
		color.diffuse = utility::math::lerp(colors[0].diffuse, colors[1].diffuse, t);
		color.specular = utility::math::lerp(colors[0].specular, colors[1].specular, t);
		color.alpha = utility::math::lerp(colors[0].alpha, colors[1].alpha, t);
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
