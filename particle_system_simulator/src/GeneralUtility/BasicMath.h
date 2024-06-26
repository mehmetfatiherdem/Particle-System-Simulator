#pragma once

#include <utility>

namespace utility::math
{
	template<class T, class U>
	inline auto max(T t, U u)
	{
		decltype(t + u) max = t > u ? t : u;
		return max;
	}

	template<class T, class U, class V>
	inline auto max(T t, U u, V v)
	{
		decltype(t + u) max = t > u ? t : u;
		decltype(max + v) result = max > v ? max : v;
		return result;
	}

	template<class T, class U>
	inline auto min(T t, U u)
	{
		decltype(t + u) min = t < u ? t : u;
		return min;
	}

	template<class T, class U, class V>
	inline auto min(T t, U u, V v)
	{
		decltype(t + u) min = t < u ? t : u;
		decltype(min + v) result = min < v ? min : v;
		return result;
	}

	template<class T>
	inline void swapToPreserveMinMax(T& min, T& max)
	{
		if (max < min)
		{
			std::swap(min, max);
		}
	}

	template<class T>
	inline T lerp(T a, T b, float t)
	{
		return a + (b - a) * t;
	}
}
