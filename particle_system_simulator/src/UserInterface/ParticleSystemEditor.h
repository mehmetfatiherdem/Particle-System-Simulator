#pragma once

#include <set>
#include <stdint.h>

class ImGuiIO;
class ImGuiContext;
class ParticleSystem;

class ParticleSystemEditor
{
private:
	ImGuiIO& io;
	ImGuiContext& context;
	std::set<ParticleSystem*> particleSystems;

	void addVerticalSpace(uint32_t count, bool useLargeSpaces = false);
	void renderParticleTabs();

public:
	ParticleSystemEditor();
	~ParticleSystemEditor() = default;

	void render();

	void addParticleSystem(ParticleSystem& ps);

	ImGuiContext& getContext() const { return context; }
	ImGuiIO& getIO() const { return io; }
};

