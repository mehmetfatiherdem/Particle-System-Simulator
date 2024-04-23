#pragma once

#include <stdint.h>

enum class LightSourceType : uint32_t
{
	DirectionalLight = 1u,
	PointLight,
	SpotLight,
};