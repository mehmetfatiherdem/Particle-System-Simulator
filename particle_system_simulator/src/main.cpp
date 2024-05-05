#include "RenderPipeline/Application.h"
#include "RenderPipeline/Initialization/Initialization.h"
#include "GeneralUtility/RefOrValue.h"

int main()
{
    initializeSettings();
    Application::getInstance().run();
    return 0;
}
