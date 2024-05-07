#pragma once

#include <string>

enum class ComponentMethod
{
	Constant,
	Curve,
	Random_Between_Two_Constants,
	Random_Between_Two_Curves,
};

inline std::string getMethodName(ComponentMethod method)
{
	switch (method)
	{
	case ComponentMethod::Constant: return "Constant";
	case ComponentMethod::Curve: return "Curve";
	case ComponentMethod::Random_Between_Two_Constants: return "Random Between Two Constants";
	case ComponentMethod::Random_Between_Two_Curves: return "Random Between Two Curves";
	default: throw 100;
	}
}