#include "RenderPipeline/Application.h"
#include "Input.h"

bool Input::getKey(KeyCode keyCode)
{
	Action action = Application::getInstance().getWindow().getKey(keyCode);
	return action == Action::PRESSED || action == Action::HELD_DOWN;
}

bool Input::getKeyDown(KeyCode keyCode)
{
	Action action = Application::getInstance().getWindow().getKey(keyCode);
	return action == Action::PRESSED;
}

bool Input::getKeyUp(KeyCode keyCode)
{
	Action action = Application::getInstance().getWindow().getKey(keyCode);
	return action == Action::RELEASED;
}

bool Input::getMouseButton(MouseButton button)
{
	Action action = Application::getInstance().getWindow().getMouseButton(button);
	return action == Action::PRESSED || action == Action::HELD_DOWN;
}

bool Input::getMouseButtonDown(MouseButton button)
{
	Action action = Application::getInstance().getWindow().getMouseButton(button);
	return action == Action::PRESSED;
}

bool Input::getMouseButtonUp(MouseButton button)
{
	Action action = Application::getInstance().getWindow().getMouseButton(button);
	return action == Action::RELEASED;
}

glm::vec2 Input::getMousePosition()
{
	return Application::getInstance().getWindow().getMousePosition();
}

float Input::getMouseX()
{
	return Application::getInstance().getWindow().getMouseX();
}

float Input::getMouseY()
{
	return Application::getInstance().getWindow().getMouseY();
}

glm::vec2 Input::getMouseDelta()
{
	return Application::getInstance().getWindow().getMouseDelta();
}

float Input::getMouseDeltaX()
{
	return Application::getInstance().getWindow().getMouseDeltaX();
}

float Input::getMouseDeltaY()
{
	return Application::getInstance().getWindow().getMouseDeltaY();
}

float Input::getScroll()
{
	return Application::getInstance().getWindow().getScroll();
}
