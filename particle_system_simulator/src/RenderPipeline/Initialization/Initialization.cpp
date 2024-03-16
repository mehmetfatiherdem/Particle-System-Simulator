#include <iostream>
#include "Initialization.h"
#include "RenderPipeline/Camera/Camera.h"

GLFWwindow* Initialization::initializeGLFW()
{
	if(!glfwInit())
	{
		std::cerr << "GLFW initialization failed!\n";
		glfwTerminate();
		return nullptr;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* tempWindow = glfwCreateWindow(480, 480, "Temp", NULL, NULL);

	if(!tempWindow)
	{
		std::cerr << "GLFW initialization failed!\n";
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(tempWindow);

	return tempWindow;
}

void Initialization::initializeGLEW()
{
	glewExperimental = GL_TRUE;     //Enable modern extension features

	if(glewInit() != GLEW_OK)   //Initializes the GLEW and returns a success value
	{
		std::cerr << "GLEW initialization failed!\n";
		glfwTerminate();
		return;
	}
	
	return;
}

void Initialization::initializeOpenGL()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	//glPolygonOffset(0.001f, 1.0f);
	glEnable(GL_CULL_FACE);
	//glEnable(GL_ALPHA);
}

void Initialization::initializeAll()
{
	initializeOpenGL();
	GLFWwindow* tempWindow = initializeGLFW();
	initializeGLEW();
	glfwDestroyWindow(tempWindow);
}