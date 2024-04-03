#include <iostream>
#include "Input Management/Utility/InputUtility.h"
#include "Window.h"

Window::Window(unsigned int width, unsigned int height, std::string_view title, bool cursorEnabled, bool escapeCloses) : width(width), height(height),
	cursorEnabled(cursorEnabled), escapeCloses(escapeCloses), mousePos(0.0f, 0.0f), mousePosPrev(0.0f, 0.0f), scroll(0.0f), keys(), mouseButtons(),
	window(nullptr)
{
	window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

	if(!window)
	{
		std::cerr << "Window creation failed!\n";
		glfwTerminate();
		window = nullptr;
		return;
	}

	int bufferWidth, bufferHeight;

	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
	glfwMakeContextCurrent(window);
	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetFramebufferSizeCallback(window, resizeCallback);

	if(!cursorEnabled)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	mousePos = glm::vec2{mouseX, mouseY};
	mousePosPrev = mousePos;

	std::vector<KeyCode> allKeys = getAllKeyCodes();
	std::vector<MouseButton> allMouseButtons = getAllMouseButtons();

	for(auto keyCode : allKeys)
	{
		keys.insert(std::make_pair(keyCode, Action::IN_REST));
	}

	for(auto mouseButton : allMouseButtons)
	{
		mouseButtons.insert(std::make_pair(mouseButton, Action::IN_REST));
	}
}

Window::~Window()
{
	glfwDestroyWindow(window);
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
	//TODO
}

void Window::keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mode)
{
	Window* ownerWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	KeyCode keyCode = toKeyCode(key);

	if(action == GLFW_PRESS)
	{
		if(ownerWindow->escapeCloses && keyCode == KeyCode::ESCAPE)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		ownerWindow->keys[keyCode] = Action::PRESSED;
	}
	else if(action == GLFW_RELEASE)
	{
		ownerWindow->keys[keyCode] = Action::RELEASED;
	}
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	Window* ownerWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if(action == GLFW_PRESS)
	{
		ownerWindow->mouseButtons[toMouseButton(button)] = Action::PRESSED;
	}
	else if(action == GLFW_RELEASE)
	{
		ownerWindow->mouseButtons[toMouseButton(button)] = Action::RELEASED;
	}
}

void Window::cursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
	Window* ownerWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	ownerWindow->mousePosPrev = ownerWindow->mousePos;
	ownerWindow->mousePos = glm::vec2{xPos, yPos};
}

void Window::scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	Window* ownerWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	ownerWindow->scroll = yOffset;
}

void Window::pollEvents()
{
	glfwPollEvents();
}

bool Window::shouldClose() const
{
	return glfwWindowShouldClose(window);
}

void Window::swapBuffers() const
{
	glfwSwapBuffers(window);
}

void Window::endFrame()
{
	for(auto iterator : keys)
	{
		switch(iterator.second)
		{
		case Action::PRESSED:
			iterator.second = Action::HELD_DOWN;
			break;
		case Action::RELEASED:
			iterator.second = Action::IN_REST;
			break;
		}
	}

	for(auto iterator : mouseButtons)
	{
		switch(iterator.second)
		{
		case Action::PRESSED:
			iterator.second = Action::HELD_DOWN;
			break;
		case Action::RELEASED:
			iterator.second = Action::IN_REST;
			break;
		}
	}

	scroll = 0;
}
