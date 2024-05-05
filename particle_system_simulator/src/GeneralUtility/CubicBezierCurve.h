#pragma once

template<typename T>
struct CubicBezierCurve
{
	T p0;
	T p1;
	T p2;
	T p3;

	T evaluate(float t) const
	{
		float u = 1.0f - t;
		float tt = t * t;
		float uu = u * u;
		float uuu = uu * u;
		float ttt = tt * t;

		return (uuu * p0) + (3 * uu * t * p1) + (3 * u * tt * p2) + (ttt * p3);
	}
};