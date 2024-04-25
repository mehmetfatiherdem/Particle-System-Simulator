#pragma once

#include <functional>
#include <stdint.h>

namespace utility::array
{
	template<typename T>
	T* find(const T* array, const T& element, uint32_t range)
	{
		auto end = array + range;
		const T* elementFound = std::find(array, end, element);

		return (elementFound != end ? elementFound : nullptr);
	}

	template<typename T>
	int findIndex(const T* array, const T& element, uint32_t range)
	{
		auto end = array + range;
		const T* elementFound = std::find(array, end, element);

		return (elementFound != end ? elementFound - array : -1);
	}
}
