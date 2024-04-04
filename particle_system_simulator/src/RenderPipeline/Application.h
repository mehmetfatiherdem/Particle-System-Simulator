#pragma once

#include "RenderPipeline/Scene/Scene.h"
#include "UserInterface/Window.h"

class Application
{
private:
    Window window;
    Scene scene;
    const unsigned int clearMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

public:
    Application();

    static Application& getInstance();

    void run();

    Window& getWindow() { return this->window; }
    Scene& getScene() { return this->scene; }
};