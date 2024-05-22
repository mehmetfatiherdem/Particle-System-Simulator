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
	default: throw "Invalid Component Type";
	}
}

inline ComponentType getComponentType(const std::string& name)
{
	if (name == "Velocity Over Lifetime")
	{
		return ComponentType::Velocity_Over_Lifetime;
	}
	else if (name == "Size Over Lifetime")
	{
		return ComponentType::Size_Over_Lifetime;
	}
	else if (name == "Size By Speed")
	{
		return ComponentType::Size_By_Speed;
	}
	else if (name == "Rotation By Speed")
	{
		return ComponentType::Rotation_By_Speed;
	}
	else if (name == "Limit Velocity Over Lifetime")
	{
		return ComponentType::Limit_Velocity_Over_Lifetime;
	}
	else if (name == "Force Over Lifetime")
	{
		return ComponentType::Force_Over_Lifetime;
	}
	else if (name == "Color Over Lifetime")
	{
		return ComponentType::Color_Over_Lifetime;
	}
	else if (name == "Color By Speed")
	{
		return ComponentType::Color_By_Speed;
	}

	throw "Invalid Component Type Name";
}