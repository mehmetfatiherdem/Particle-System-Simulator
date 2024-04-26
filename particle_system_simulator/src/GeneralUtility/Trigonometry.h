#pragma once

#include <cmath>
#include <glm/vec2.hpp>
#include "MathConstants.h"

namespace Trig
{
	double atan2(double y, double x)
	{
		double result = std::atan2(y, x);
		return result < 0 ? result + TAU : result;
	}

	double atan2(glm::vec2 direction)
	{
		return Trig::atan2(direction.y, direction.x);
	}
}
