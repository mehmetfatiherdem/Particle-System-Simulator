#pragma once

template <typename Number>
inline Number sign(Number number)
{
	return static_cast<Number>((number >= 0) - (number < 0));
}