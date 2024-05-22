#pragma once

#include <string>

enum class EmitterType
{
	SphereEmitter,
	ConeEmitter,
};

inline std::string getEmitterTypeName(EmitterType type)
{
	switch (type)
	{
	case EmitterType::SphereEmitter: return "Sphere Emitter";
	case EmitterType::ConeEmitter: return "Cone Emitter";
	default: throw "Invalid Emitter Type";
	}
}

inline EmitterType getEmitterType(const std::string& name)
{
	if (name == "Sphere Emitter")
	{
		return EmitterType::SphereEmitter;
	}
	else if (name == "Cone Emitter")
	{
		return EmitterType::ConeEmitter;
	}

	throw "Invalid Emitter Type Name";
}