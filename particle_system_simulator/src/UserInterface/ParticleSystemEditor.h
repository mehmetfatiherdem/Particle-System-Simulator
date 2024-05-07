#pragma once

#include <set>
#include <stdint.h>
#include <glm/glm.hpp>
#include "GeneralUtility/CubicBezierCurve.h"
#include "imgui/imgui.h"

class ImGuiIO;
class ImGuiContext;
class ParticleSystem;
class Emitter;
class Component;
class Color4;

class ParticleSystemEditor
{
private:
	ImGuiIO& io;
	ImGuiContext& context;
	std::set<ParticleSystem*> particleSystems;

	void addVerticalSpace(uint32_t count, bool useLargeSpaces = false);
	void renderParticleTabs();
	void renderEmitter(ParticleSystem* ps);
	void renderComponents(ParticleSystem* ps);

	void renderBezierVector(const std::string& name, CubicBezierCurve<glm::vec3>* bezier, CubicBezierCurve<glm::vec3>* copy = nullptr);
	void renderBezierFloat(const std::string& name, CubicBezierCurve<float>* bezier, CubicBezierCurve<float>* copy = nullptr);

	void renderColor(const std::string& name, Color4* color, ImGuiTreeNodeFlags_ flags = ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_None);

	void renderMinMaxFloat(const std::string& name, float* min, float* max, float minLimit = 0.0f);
	void renderMinMaxFloat(const std::string& minName, const std::string& maxName, float* min, float* max, float minLimit = 0.0f);
	void renderMinMaxVector(const std::string& name, glm::vec3* min, glm::vec3* max, const glm::vec3& minLimit = glm::vec3{0.0f, 0.0f, 0.0f});

	int getEmitterType(const Emitter* emitter) const;
public:
	ParticleSystemEditor();
	~ParticleSystemEditor() = default;

	void render();

	void addParticleSystem(ParticleSystem& ps);

	ImGuiContext& getContext() const { return context; }
	ImGuiIO& getIO() const { return io; }
};

