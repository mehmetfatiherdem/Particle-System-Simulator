#include "Persistence/Serializer.h"
#include "Component.h"

void Component::tryUpdate(const ParticleSystemProps& props, Particle& particle)
{
	if (!enabled) return;
	update(props, particle);
}

void Component::serialize(Serializer& serializer, const std::string& objectName) const
{
	serializer.startObject(objectName);
	serializer["ComponentType"].String(getComponentTypeName(getType()).c_str());
	serializer["Priority"].Uint(priority);
	serializer["Enabled"].Bool(enabled);
}