#pragma once

#include <string>

enum class ComponentType
{
	Velocity_Over_Lifetime = 0,
	Size_Over_Lifetime,
	Size_By_Speed,
	Rotation_By_Speed,
	Limit_Velocity_Over_Lifetime,
	Force_Over_Lifetime,
	Color_Over_Lifetime,
	Color_By_Speed,
};

inline std::string getComponentTypeName(ComponentType type)
{
	switch (type)
	{
	case ComponentType::Velocity_Over_Lifetime: return "Velocity Over Lifetime";
	case ComponentType::Size_Over_Lifetime: return "Size Over Lifetime";
	case ComponentType::Size_By_Speed: return "Size By Speed";
	case ComponentType::Rotation_By_Speed: return "Rotation By Speed";
	case ComponentType::Limit_Velocity_Over_Lifetime: return "Limit Velocity Over Lifetime";
	case ComponentType::Force_Over_Lifetime: return "Force Over Lifetime";
	case ComponentType::Color_Over_Lifetime: return "Color Over Lifetime";
	case ComponentType::Color_By_Speed: return "Color By Speed";
	default: throw 100;
	}
}