#pragma once

template <typename Number>
Number sign(Number number)
{
	return static_cast<Number>((number >= 0) - (number < 0));
}