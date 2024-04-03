#pragma once

#include <glm/vec2.hpp>
#include <map>
#include "Data/KeyCode.h"
#include "Data/MouseButton.h"

class Input
{
public:
	static bool getKey(KeyCode keyCode);		//pressed || held
	static bool getKeyDown(KeyCode keyCode);		//pressed
	static bool getKeyUp(KeyCode keyCode);		//released
	static bool getMouseButton(MouseButton button);
	static bool getMouseButtonDown(MouseButton button);
	static bool getMouseButtonUp(MouseButton button);
	static glm::vec2 getMousePosition();
	static float getMouseX();
	static float getMouseY();
	static glm::vec2 getMouseDelta();
	static float getMouseDeltaX();
	static float getMouseDeltaY();
	static float getScroll();
};
