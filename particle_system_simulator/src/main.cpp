#include "RenderPipeline/Application.h"
#include "RenderPipeline/Initialization/Initialization.h"
#include <iostream>

int main()
{
    initializeSettings();
    Application::getInstance().run();
    return 0;
}
