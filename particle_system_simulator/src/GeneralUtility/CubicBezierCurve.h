#pragma once

template<typename T>
struct CubicBezierCurve
{
	T p0 = default(T), p1 = default(T), p2 = default(T), p3 = default(T);

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