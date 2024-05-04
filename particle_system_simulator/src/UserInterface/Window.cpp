#include <iostream>
#include <cmath>
#include "Input Management/Utility/InputUtility.h"
#include "RenderPipeline/Application.h"
#include "Window.h"

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
	GLsizei length, const char* message, const void* userParam);

Window::Window(uint32_t width, uint32_t height, std::string_view title, bool vsync, bool cursorEnabled, bool escapeCloses) :
	width(width), height(height), vsync(vsync), cursorEnabled(cursorEnabled), escapeCloses(escapeCloses), mousePos(0.0f, 0.0f),
	mouseDelta(0.0f, 0.0f), scroll(0.0f), keys(), mouseButtons(), window(nullptr)
{
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
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

	glfwSwapInterval(vsync);

	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetFramebufferSizeCallback(window, resizeCallback);

	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		//glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	glfwSetInputMode(window, GLFW_CURSOR, cursorEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	mousePos = glm::vec2{mouseX, mouseY};

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
	Window* ownerWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	ownerWindow->width = width;
	ownerWindow->height = height;
	Application::getInstance().getScene().getCamera().setAspectRatio(width, height);

	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
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
	glm::vec2 newPos = glm::vec2{xPos / ownerWindow->width, yPos / ownerWindow->height};
	ownerWindow->mouseDelta += (newPos - ownerWindow->mousePos);
	ownerWindow->mousePos = newPos;
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
	for(auto iterator = keys.begin(); iterator != keys.end(); ++iterator)
	{
		switch(iterator._Ptr->_Myval.second)
		{
		case Action::PRESSED:
			iterator._Ptr->_Myval.second = Action::HELD_DOWN;
			break;
		case Action::RELEASED:
			iterator._Ptr->_Myval.second = Action::IN_REST;
			break;
		}
	}

	for(auto iterator = mouseButtons.begin(); iterator != mouseButtons.end(); ++iterator)
	{
		switch(iterator._Ptr->_Myval.second)
		{
		case Action::PRESSED:
			iterator._Ptr->_Myval.second = Action::HELD_DOWN;
			break;
		case Action::RELEASED:
			iterator._Ptr->_Myval.second = Action::IN_REST;
			break;
		}
	}

	scroll = 0;
	mouseDelta = glm::vec2{0.0f, 0.0f};
}

void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char* message,
	const void* userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}
