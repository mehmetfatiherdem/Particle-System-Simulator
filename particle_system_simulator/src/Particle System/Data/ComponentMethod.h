#pragma once

#include <string>

enum class ComponentMethod
{
	Constant,
	Curve,
	Random_Between_Two_Constants,
	Random_Between_Two_Curves,
};

inline std::string getComponentMethodName(ComponentMethod method)
{
	switch (method)
	{
	case ComponentMethod::Constant: return "Constant";
	case ComponentMethod::Curve: return "Curve";
	case ComponentMethod::Random_Between_Two_Constants: return "Random Between Two Constants";
	case ComponentMethod::Random_Between_Two_Curves: return "Random Between Two Curves";
	default: throw "Invalid Component Method Type";
	}
}

inline ComponentMethod getComponentMethodType(const std::string& name)
{
	if (name == "Curve")
	{
		return ComponentMethod::Curve;
	}
	else if (name == "Random Between Two Constants")
	{
		return ComponentMethod::Random_Between_Two_Constants;
	}
	else if (name == "Random Between Two Curves")
	{
		return ComponentMethod::Random_Between_Two_Curves;
	}
	else if (name == "Constant")
	{
		return ComponentMethod::Constant;
	}

	throw "Invalid Component Method Type Name";
}