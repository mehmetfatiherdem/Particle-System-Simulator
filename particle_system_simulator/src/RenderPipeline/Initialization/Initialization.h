#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Initialization
{
	GLFWwindow* initializeGLFW();
	void initializeGLEW();
	void initializeOpenGL();
	void initializeAll();
}