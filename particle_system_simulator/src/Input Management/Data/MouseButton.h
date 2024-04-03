#pragma once

#include <GLFW/glfw3.h>

enum class MouseButton
{
	MOUSE_LEFT = GLFW_MOUSE_BUTTON_LEFT,
	MOUSE_RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
	MOUSE_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE,
};

constexpr unsigned int castMouseButton(MouseButton button)
{
	return static_cast<unsigned int>(button);
}
