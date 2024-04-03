#pragma once

#include <string_view>
#include <map>
#include <glm/vec2.hpp>
#include "Input Management/Data/KeyCode.h"
#include "Input Management/Data/MouseButton.h"
#include "Input Management/Data/Action.h"

class GLFWwindow;

class Window
{
private:
	GLFWwindow* window;

	unsigned int width;
	unsigned int height;
	bool cursorEnabled;
	bool escapeCloses;
	
	std::map<KeyCode, Action> keys;
	std::map<MouseButton, Action> mouseButtons;

	glm::vec2 mousePos;
	glm::vec2 mousePosPrev;
	float scroll;

	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mode);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);
	static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

public:
	Window() = delete;
	Window(unsigned int width, unsigned int height, std::string_view title, bool cursorEnabled = true, bool escapeCloses = true);
	~Window();

	static void pollEvents();
	bool shouldClose() const;
	void swapBuffers() const;
	void endFrame();

	glm::vec2 getMousePosition() const { return mousePos; }
	float getMouseX() const { return mousePos.x; }
	float getMouseY() const { return mousePos.y; }
	glm::vec2 getMouseDelta() const { return mousePos - mousePosPrev; }
	float getMouseDeltaX() const { return (mousePos - mousePosPrev).x; }
	float getMouseDeltaY() const { return (mousePos - mousePosPrev).y; }
	float getScroll() const { return scroll; }

	unsigned int getWidth() const { return width; }
	unsigned int getHeight() const { return height; }
	float getAspectRatio() const { return (float)width / height; }

	Action getKey(KeyCode keyCode) const { return keys.at(keyCode); }
	Action getMouseButton(MouseButton button) const { return mouseButtons.at(button); }
};