#include "Editor.h"

Editor::Editor()
{
	createContext();
	enableIOForKeyboard();
}

void Editor::createContext()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	context = ImGui::GetCurrentContext();
	io = &(ImGui::GetIO());
}

void Editor::setCurrentContext(ImGuiContext* currentContext)
{
	ImGui::SetCurrentContext(currentContext);
}

void Editor::enableIOForKeyboard()
{
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
}

void Editor::beginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Editor::showDemoWindow()
{
	ImGui::ShowDemoWindow();
}

void Editor::render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::endFrame()
{
	ImGui::EndFrame();
}

void Editor::shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Editor::addWindow(const char* name, bool* isOpen, ImGuiWindowFlags flags)
{
	ImGui::Begin(name, isOpen, flags);
}

void Editor::endWindow()
{
	ImGui::End();
}

void Editor::addChildWindow(const char* name)
{
	ImGui::BeginChild(name);
}

void Editor::endChildWindow()
{
	ImGui::EndChild();
}

void Editor::editColor3(const char* name, float* color)
{
	ImGui::ColorEdit3(name, color);
}

void Editor::editColor4(const char* name, float* color)
{
	ImGui::ColorEdit4(name, color);
}

Editor::~Editor()
{
	shutdown();
}
