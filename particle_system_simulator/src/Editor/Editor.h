#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

/*
		window is a type of GLFWwindow*. and this setsup imgui for the platform
	    where to put this??
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();

*/

class Editor
{
private:
	ImGuiIO* io;
	ImGuiContext* context;

public:
	Editor();

	void createContext();
	void setCurrentContext(ImGuiContext* currentContext);
	void enableIOForKeyboard();
	void beginFrame();
	void showDemoWindow(); // demo window is a prewritten window that shows all the features of imgui
	void render();
	void endFrame();
	void shutdown();

	void addWindow(const char* name, bool* isOpen, ImGuiWindowFlags flags = 0);
	void endWindow(); // call this at last after adding all the elements to the window
	void addChildWindow(const char* name);
	void endChildWindow(); // call this at last after adding all the elements to the child window
	void editColor3(const char* name, float* color);
	void editColor4(const char* name, float* color);

	ImGuiContext* getContext() const { return context; }
	ImGuiIO* getIO() const { return io; }

	~Editor();
};

