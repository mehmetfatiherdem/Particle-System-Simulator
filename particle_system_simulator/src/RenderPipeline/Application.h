#pragma once

#include <stdint.h>
#include <functional>
#include "RenderPipeline/Scene/Scene.h"
#include "UserInterface/Window.h"
#include "UserInterface/ParticleSystemEditor.h"

class Application
{
private:
    Window window;
    Scene scene;
	ParticleSystemEditor editor;
    const uint32_t clearMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

    void gameLoop(std::function<void()> gameLoop);

public:
    Application();

    static Application& getInstance();

    void run();

    Window& getWindow() { return this->window; }
    Scene& getScene() { return this->scene; }
};