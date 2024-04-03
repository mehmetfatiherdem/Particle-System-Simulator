#pragma once

#include <glm/vec2.hpp>
#include "Data/Keycode.h"
#include "Data/MouseButton.h"

class Input
{
public:
	static bool isKeyPressed(Keycode keycode);
	static bool isMouseButtonPressed(MouseButton button);
	static glm::vec2 getMousePosition();
	static float getMouseScroll();
	static float getMouseX();
	static float getMouseY();
};
