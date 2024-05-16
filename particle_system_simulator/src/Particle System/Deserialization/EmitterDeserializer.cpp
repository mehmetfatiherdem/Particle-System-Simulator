#include "Particle System/Emitter/SphereEmitter.h"
#include "Particle System/Emitter/ConeEmitter.h"
#include "EmitterDeserializer.h"
#include "Persistence/Deserializer.h"

std::unique_ptr<Emitter> deserializeEmitter(const Deserializer& deserializer)
{
	float emissionRate = deserializer["EmissionRate"].getReal();

	switch (getEmitterType(deserializer["EmitterType"].getString()))
	{
	case EmitterType::SphereEmitter:
	{
		float radius = deserializer["Radius"].getReal();
		float arc = deserializer["Arc"].getReal();
		return std::make_unique<SphereEmitter>(emissionRate, radius, arc);
	}
	case EmitterType::ConeEmitter:
	{
		float radius = deserializer["Radius"].getReal();
		float angle = deserializer["Angle"].getReal();
		return std::make_unique<ConeEmitter>(emissionRate, radius, angle);
	}
	default: throw "Invalid Emitter Type";
	}
}
