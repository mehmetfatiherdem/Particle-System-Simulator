#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "Gui.h"

void Gui::init(void* nativeWindow)
{
	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)nativeWindow, true);
	ImGui_ImplOpenGL3_Init();
}

void Gui::beginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Gui::endFrame()
{
	ImGui::EndFrame();
}

void Gui::shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}