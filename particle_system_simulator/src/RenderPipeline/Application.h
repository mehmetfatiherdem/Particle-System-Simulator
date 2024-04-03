#pragma once

#include "UserInterface/Window.h"
#include "RenderPipeline/Scene/Scene.h"

class Application
{
private:
    Window window;
    Scene scene;

    Application();
public:

    static Application& getInstance()
    {
        static Application app;
        return app;
    }

    void run();
    void update();


    Window& getWindow() { return this->window; }
    Scene& getScene() { return this->scene; }
};