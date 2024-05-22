#include "RenderPipeline/Application.h"
#include "RenderPipeline/Configuration/Configuration.h"
#include "UserInterface/Gui.h"
#include "GeneralUtility/Random.h"

int main()
{
	Random::init();
    Configuration::init();
	Gui::init();

	Application& app = Application::getInstance();
	Gui::initWindow(app.getWindow().getNativeWindow());
	app.run();

	Gui::shutdown();
	Configuration::shutdown();
    return 0;
}
