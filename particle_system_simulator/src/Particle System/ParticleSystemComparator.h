#pragma once

#include <string>
#include "ParticleSystem.h"

class ParticleSystemComparator
{
public:
	bool operator()(const ParticleSystem& lhs, const ParticleSystem& rhs) const noexcept
	{
		return lhs.getName() < rhs.getName();
	}
};