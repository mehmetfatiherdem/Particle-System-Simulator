#pragma once

#include <utility>

namespace utility::math
{
	template<class T, class U, class V>
	auto max(T t, U u, V v)
	{
		decltype(t + u) max = t > u ? t : u;
		decltype(max + v) result = max > v ? max : v;
		return result;
	}

	template<class T>
	void swapToPreserveMinMax(T& min, T& max)
	{
		if (max < min)
		{
			std::swap(min, max);
		}
	}
}
