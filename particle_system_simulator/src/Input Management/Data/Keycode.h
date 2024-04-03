#pragma once

#include <GLFW/glfw3.h>

enum class Keycode
{
	KEY_W = GLFW_KEY_W,
	KEY_A = GLFW_KEY_A,
	KEY_S = GLFW_KEY_S,
	KEY_D = GLFW_KEY_D,
	KEY_Q = GLFW_KEY_Q,
	KEY_E = GLFW_KEY_E,
	KEY_R = GLFW_KEY_R,
	KEY_T = GLFW_KEY_T,
	KEY_Y = GLFW_KEY_Y,
	KEY_U = GLFW_KEY_U,
	KEY_F = GLFW_KEY_F,
	KEY_Z = GLFW_KEY_Z,
	KEY_X = GLFW_KEY_X,
	KEY_C = GLFW_KEY_C,
	ARROW_UP = GLFW_KEY_PAGE_UP,
	ARROW_LEFT = GLFW_KEY_HOME,
	ARROW_BACK = GLFW_KEY_PAGE_DOWN,
	ARROW_RIGHT = GLFW_KEY_END,
	SHIFT = GLFW_KEY_LEFT_SHIFT,
	CTRL = GLFW_KEY_LEFT_CONTROL,
	ESCAPE = GLFW_KEY_ESCAPE,
	SPACE = GLFW_KEY_SPACE,
};

constexpr unsigned int castKeycode(Keycode keycode)
{
	return static_cast<unsigned int>(keycode);
}