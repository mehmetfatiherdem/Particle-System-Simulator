#pragma once

namespace Gui
{
	void init();
	void initWindow(void* nativeWindow);
	void beginFrame();
	void endFrame();
	void shutdown();

	bool wantCaptureMouse();
	bool wantCaptureKeyboard();
}