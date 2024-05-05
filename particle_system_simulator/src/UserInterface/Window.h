#pragma once

#include <string_view>
#include <map>
#include <stdint.h>

#include <glm/vec2.hpp>
#include "Input Management/Data/KeyCode.h"
#include "Input Management/Data/MouseButton.h"
#include "Input Management/Data/Action.h"

class GLFWwindow;

class Window
{
private:
	GLFWwindow* window;

	uint32_t width;
	uint32_t height;
	bool cursorEnabled;
	bool escapeCloses;
	bool vsync;
	
	std::map<KeyCode, Action> keys;
	std::map<MouseButton, Action> mouseButtons;

	glm::vec2 mousePos;
	glm::vec2 mouseDelta;
	float scroll;

	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mode);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);
	static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

public:
	Window() = delete;
	Window(uint32_t width, uint32_t height, std::string_view title, bool vsync = false, bool cursorEnabled = true,
		bool escapeCloses = true);
	~Window();

	static void pollEvents();
	bool shouldClose() const;
	void swapBuffers() const;
	void endFrame();

	void* getNativeWindow() const { return window; }

	glm::vec2 getMousePosition() const { return mousePos; }
	float getMouseX() const { return mousePos.x; }
	float getMouseY() const { return mousePos.y; }
	glm::vec2 getMouseDelta() const { return mouseDelta; }
	float getMouseDeltaX() const { return mouseDelta.x; }
	float getMouseDeltaY() const { return mouseDelta.y; }
	float getScroll() const { return scroll; }
	bool getVsync() const { return vsync; }

	uint32_t getWidth() const { return width; }
	uint32_t getHeight() const { return height; }
	float getAspectRatio() const { return (float)width / height; }

	Action getKey(KeyCode keyCode) const { return keys.at(keyCode); }
	Action getMouseButton(MouseButton button) const { return mouseButtons.at(button); }
};