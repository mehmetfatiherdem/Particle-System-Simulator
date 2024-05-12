#pragma once

#include <stdint.h>
#include <set>
#include <functional>
#include "RenderPipeline/Scene/Scene.h"
#include "UserInterface/Window.h"
#include "Particle System/ParticleSystem.h"
#include "Particle System/ParticleSystemComparator.h"
#include "UserInterface/ParticleSystemEditor.h"

class Application
{
private:
    Window window;
    Scene scene;
    std::set<ParticleSystem, ParticleSystemComparator> particleSystems;
	ParticleSystemEditor editor;
    const uint32_t clearMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

public:
    Application();

    static Application& getInstance();

    void run();

	void addParticleSystem(ParticleSystem&& particleSystem) { this->particleSystems.insert(std::move(particleSystem)); }
	void removeParticleSystem(const ParticleSystem& particleSystem) { this->particleSystems.erase(particleSystem); }
    std::set<ParticleSystem, ParticleSystemComparator>& getParticleSystems() { return particleSystems; }
    Window& getWindow() { return this->window; }
    Scene& getScene() { return this->scene; }
	ParticleSystemEditor& getEditor() { return this->editor; }
};