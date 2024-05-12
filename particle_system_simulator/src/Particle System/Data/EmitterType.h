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
	default: throw 100;
	}
}