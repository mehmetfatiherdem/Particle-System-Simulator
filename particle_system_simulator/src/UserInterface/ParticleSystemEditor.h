#pragma once

#include <vector>

class ImGuiIO;
class ImGuiContext;
class ParticleSystem;

class ParticleSystemEditor
{
private:
	ImGuiIO& io;
	ImGuiContext& context;
	std::vector<ParticleSystem*> particleSystems;

public:
	ParticleSystemEditor();
	~ParticleSystemEditor() { shutdown(); }

	void render();
	void shutdown();

	void addParticleSystem(ParticleSystem& ps);

	ImGuiContext& getContext() const { return context; }
	ImGuiIO& getIO() const { return io; }
};

