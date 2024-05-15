#pragma once

#include <vector>
#include <cassert>
#include <stdint.h>
#include <tuple>
#include <glm/vec3.hpp>
#include "MathConstants.h"

template<typename T>
class BezierCurve
{
private:
	std::vector<T> controlPoints;

public:
	BezierCurve() : controlPoints() {}

	size_t size() const { return controlPoints.size(); }

	std::vector<T>::iterator begin() { return controlPoints.begin(); }
	std::vector<T>::iterator end() { return controlPoints.end(); }
	std::vector<T>::const_iterator cbegin() const { return controlPoints.cbegin(); }
	std::vector<T>::const_iterator cend() const { return controlPoints.cend(); }

	void pushControlPoint(T value) { controlPoints.push_back(value); }

	bool addControlPoint(int index, T value)
	{
		if (index < -1 || index >= controlPoints.size()) return false;
		controlPoints.insert(controlPoints.begin() + index + 1, value);
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

	T evaluatePoint(float t) const
	{
		assert(t >= 0.0f && t <= 1.0f + EPSILON);

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
}
