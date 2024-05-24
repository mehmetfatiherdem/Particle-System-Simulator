#pragma once

#include <vector>
#include <cassert>
#include <stdint.h>
#include <tuple>
#include <functional>
#include <glm/vec3.hpp>
#include "MathConstants.h"

template<typename T>
class BezierCurve
{
private:
	std::vector<T> controlPoints;

public:
	BezierCurve() : controlPoints() {}

	void clear() { controlPoints.clear(); }
	size_t size() const { return controlPoints.size(); }

	std::vector<T>::iterator begin() { return controlPoints.begin(); }
	std::vector<T>::iterator end() { return controlPoints.end(); }
	std::vector<T>::const_iterator cbegin() const { return controlPoints.cbegin(); }
	std::vector<T>::const_iterator cend() const { return controlPoints.cend(); }

	void pushControlPoint(T value) { controlPoints.push_back(value); }

	bool addControlPoint(int index, T value)
	{
		if (index < -1 || index >= static_cast<int>(controlPoints.size())) return false;
		controlPoints.insert(controlPoints.begin() + 1 + index, value);
		return true;
	}

	bool deleteControlPoint(size_t index)
	{
		if (index >= controlPoints.size()) return false;
		controlPoints.erase(controlPoints.begin() + index);
		return true;
	}

	T& operator[](size_t index)
	{
		if (index >= controlPoints.size())
		{
			throw "Index Out Of Range!";
		}

		return controlPoints[index];
	}

	const T& operator[](size_t index) const
	{
		if (index >= controlPoints.size())
		{
			throw "Index Out Of Range!";
		}

		return controlPoints[index];
	}

	bool validate(std::function<bool(T)> validator) const
	{
		float stepSize = 1.0f / (49);
		float t = 0.0f;

		for (uint32_t i = 0; i < 50; ++i, t += stepSize)
		{
			if (!validator(evaluatePoint(t))) return false;
		}

		return true;
	}

	T evaluatePoint(float t) const
	{
		//assert(t >= 0.0f && t <= 1.0f + EPSILON);

		std::vector<T> beta(controlPoints);
		size_t n = beta.size();

		for (size_t i = 1; i <= n; ++i)
		{
			for (size_t j = 0; j < (n - i); ++j)
			{
				beta[j] = beta[j] * (1 - t) + beta[j + 1] * t;
			}
		}

		return beta[0];
	}

	std::tuple<std::vector<float>, std::vector<T>> evaluateCurve(uint32_t nrSteps) const
	{
		assert(nrSteps >= 2);

		std::vector<float> time;
		std::vector<T> curve;

		curve.reserve(nrSteps);
		time.reserve(nrSteps);

		float stepSize = 1.0f / (nrSteps - 1);
		float t = 0.0f;

		for (uint32_t i = 0; i < nrSteps; ++i, t += stepSize)
		{
			time.push_back(t);
			curve.push_back(evaluatePoint(t));
		}

		return std::make_tuple(std::move(time), std::move(curve));
	}
};

namespace Bezier
{
	static BezierCurve<float> createLinear()
	{
		BezierCurve<float> curve;
		curve.pushControlPoint(0.0f);
		curve.pushControlPoint(1.0f);
		return curve;
	}

	static BezierCurve<glm::vec3> createLinearVec3()
	{
		BezierCurve<glm::vec3> curve;
		curve.pushControlPoint(glm::vec3(0.0f, 0.0f, 0.0f));
		curve.pushControlPoint(glm::vec3(1.0f, 1.0f, 1.0f));
		return curve;
	}

	static std::function<bool(float)> bezierFloatValidator(bool addMin, bool addMax, float min, float max)
	{
		if (addMin && addMax)
		{
			return [min, max](float value) -> bool
				{
					return value >= min && value <= max;
				};
		}
		else if (addMin)
		{
			return [min](float value) -> bool
				{
					return value >= min;
				};
		}
		else if (addMax)
		{
			return [max](float value) -> bool
				{
					return value <= max;
				};
		}

		return [](float value) -> bool
			{
				return true;
			};
	}

	static std::function<bool(glm::vec3)> bezierVec3Validator(bool addMin, bool addMax, const glm::vec3& min, const glm::vec3& max)
	{
		if (addMin && addMax)
		{
			return [&min, &max](glm::vec3 value) -> bool
				{
					return value.x >= min.x && value.x <= max.x &&
						value.y >= min.y && value.y <= max.y &&
						value.z >= min.z && value.z <= max.z;
				};
		}
		else if (addMin)
		{
			return [&min](glm::vec3 value) -> bool
				{
					return value.x >= min.x &&
						value.y >= min.y &&
						value.z >= min.z;
				};
			
		}
		else if (addMax)
		{
			return [&max](glm::vec3 value) -> bool
				{
					return value.x <= max.x &&
						value.y <= max.y &&
						value.z <= max.z;
				};
		}

		return [](glm::vec3 value) -> bool
			{
				return true;
			};

	}
}
