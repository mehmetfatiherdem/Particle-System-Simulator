#include "RenderPipeline/Application.h"
#include "RenderPipeline/Initialization/Initialization.h"

int main()
{
    initializeSettings();
    Application::getInstance().run();
    return 0;
}
