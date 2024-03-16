#pragma once

#include <string_view>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

class Window
{
private:
	GLFWwindow* window;

	int width;
	int height;
	uint64_t keys[16];
	bool cursorEnabled;

	int bufferWidth;
	int bufferHeight;
	glm::vec2 mouseLastPos;
	glm::vec2 mouseDelta;
	float scrollDelta;
	bool mouseFirstMove;

	static GLfloat currentFrame;
	static GLfloat lastFrame;

	void setKeyAtIndex(int index, bool value);

	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void keyCallback(GLFWwindow* window, int key, int code, int action, int mode);
	static void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);
	static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

public:
	Window() = delete;
	Window(unsigned int width, unsigned int height, std::string_view title, bool cursorEnabled = true);
	~Window() { glfwDestroyWindow(window); }

	static void updateTime();
	static void pollEvents() { glfwPollEvents(); }
	static GLfloat getDeltaTime() { return currentFrame - lastFrame; }

	void swapBuffers() { glfwSwapBuffers(window); }

	void getMouseDelta(GLdouble* deltaX, GLdouble* deltaY);
	GLdouble getScrollDelta();
	bool getKeyAtIndex(int index) const;
	bool shouldClose() const { return glfwWindowShouldClose(window); }
	GLfloat getAspectRatio() const { return (float)width / height; }
};