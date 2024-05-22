#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/implot.h"
#include "Gui.h"

void Gui::init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void Gui::initWindow(void* nativeWindow)
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
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
}

bool Gui::wantCaptureMouse()
{
	return ImGui::GetIO().WantCaptureMouse;
}

bool Gui::wantCaptureKeyboard()
{
	return ImGui::GetIO().WantCaptureKeyboard;
}