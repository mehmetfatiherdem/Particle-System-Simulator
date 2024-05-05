#include <iostream>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "GeneralUtility/gl2fw3.h"
#include "Initialization.h"

void initializeSettings()
{
	if(!glfwInit())
	{
		std::cerr << "GLFW initialization failed!\n";
		glfwTerminate();
		return;
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
		return;
	}

	glfwMakeContextCurrent(tempWindow);

	glewExperimental = GL_TRUE;

	if(glewInit() != GLEW_OK)
	{
		std::cerr << "GLEW initialization failed!\n";
		glfwTerminate();
		return;
	}

	glfwDestroyWindow(tempWindow);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}