#include <iostream>
#include "Window.h"

GLfloat Window::currentFrame = 0.0f;
GLfloat Window::lastFrame = 0.0f;

Window::Window(unsigned int width, unsigned int height, std::string_view title, bool cursorEnabled) : width(width), height(height), cursorEnabled(cursorEnabled),
	mouseLastPos(0.0f, 0.0f), mouseDelta(0.0f, 0.0f), scrollDelta(0.0f)
{
	window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

	if(!window)
	{
		std::cerr << "Window creation failed!\n";
		glfwTerminate();
		window = nullptr;
		return;
	}

	currentFrame = 0;
	lastFrame = 0;

	for(size_t i = 0; i < 16; i++)
		keys[i] = 0;

	if(!cursorEnabled)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
	glfwMakeContextCurrent(window);
	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetFramebufferSizeCallback(window, resizeCallback);
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
	//TODO
}

void Window::keyCallback(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* ownerWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));	//Get the owner Window class from GLFW

	if(action == GLFW_PRESS)	//If the action is a key press
	{
		if(key == GLFW_KEY_ESCAPE)	//If the key pressed is Escape
			glfwSetWindowShouldClose(window, GL_TRUE);	//Set the window should close

		ownerWindow->setKeyAtIndex(key, true);
	}
	else if(action == GLFW_RELEASE)
	{
		ownerWindow->setKeyAtIndex(key, false);
	}
}

void Window::cursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
	Window* ownerWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if(ownerWindow->mouseFirstMove)
	{
		ownerWindow->mouseDelta.x = xPos - ownerWindow->mouseLastPos.x;
		ownerWindow->mouseDelta.y = ownerWindow->mouseLastPos.x - yPos;
	}
	else
	{
		ownerWindow->mouseFirstMove = true;
	}

	ownerWindow->mouseLastPos.x = xPos;
	ownerWindow->mouseLastPos.y = yPos;
}

void Window::scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	Window* ownerWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	ownerWindow->scrollDelta = yOffset;
}

void Window::updateTime()
{
	lastFrame = currentFrame;
	currentFrame = glfwGetTime();
}

void Window::setKeyAtIndex(int index, bool value)
{
	if(index < 0 || index > 1023)
		return;

	int maskIndex = index / 64;
	int digitIndex = index % 64;

	uint64_t mask = static_cast<uint64_t>(1) << digitIndex;

	if(value)
		keys[maskIndex] |= mask;
	else
		keys[maskIndex] &= (~mask);
}

void Window::getMouseDelta(GLdouble* deltaX, GLdouble* deltaY)
{
	*deltaX = mouseDelta.x;
	*deltaY = mouseDelta.y;
	mouseDelta = glm::vec2{0.0f, 0.0f};
}

GLdouble Window::getScrollDelta()
{
	GLdouble delta = scrollDelta;
	scrollDelta = 0.0f;
	return delta;
}

bool Window::getKeyAtIndex(int index) const
{
	if (index < 0 || index > 1023)
		return false;

	int maskIndex = index / 64;
	int digitIndex = index % 64;

	uint64_t num = keys[maskIndex];

	return num & (static_cast<uint64_t>(1) << digitIndex);
}
