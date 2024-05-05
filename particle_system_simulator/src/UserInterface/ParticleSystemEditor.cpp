#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "Particle System/ParticleSystem.h"
#include "ParticleSystemEditor.h"

ParticleSystemEditor::ParticleSystemEditor() : io(ImGui::GetIO()), context(*ImGui::GetCurrentContext())
{

}

//void Editor::beginFrame()
//{
//	ImGui_ImplOpenGL3_NewFrame();
//	ImGui_ImplGlfw_NewFrame();
//	ImGui::NewFrame();
//}
//
//void Editor::endFrame()
//{
//	ImGui::EndFrame();
//}

void ParticleSystemEditor::render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ParticleSystemEditor::shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ParticleSystemEditor::addParticleSystem(ParticleSystem& ps)
{
	particleSystems.push_back(&ps);
}