#pragma once

#include <set>
#include <stdint.h>
#include <limits>
#include <functional>
#include <glm/glm.hpp>
#include "RenderPipeline/Texture/Texture.h"
#include "GeneralUtility/BezierCurve.h"
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
	Texture* emptyTexture;
	bool hasFocus;
	bool isHovered;
	ImVec2 windowSize;

	void addVerticalSpace(uint32_t count, bool useLargeSpaces = false);

	void renderParticleTabs();
	void renderParticleSystem(ParticleSystem& ps);
	void renderEmitter(ParticleSystem& ps);
	void renderComponents(ParticleSystem& ps);
	void renderMainMenuBar();

	void renderBezierVector(const std::string& name, const std::string& yAxis, BezierCurve<glm::vec3>* bezier, BezierCurve<glm::vec3>* copy = nullptr,
		ImGuiTreeNodeFlags flags = 0, bool addMin = false, bool addMax = false, const glm::vec3& min = glm::vec3(0.0f, 0.0f, 0.0f),
		const glm::vec3& max = glm::vec3(0.0f, 0.0f, 0.0f));

	void renderBezierFloat(const std::string& name, const std::string& yAxis, BezierCurve<float>* bezier, BezierCurve<float>* copy = nullptr,
		ImGuiTreeNodeFlags flags = 0, bool addMin = false, bool addMax = false, float min = 0.0f, float max = 0.0f);

	void renderColor(const std::string& name, Color4* color, ImGuiTreeNodeFlags flags = 0);

	void renderMinMaxFloat(const std::string& name, float* min, float* max, float minLimit = 0.0f, float step = 0.01f, float fastStep = 0.1f);
	void renderMinMaxFloat(const std::string& minName, const std::string& maxName, float* min, float* max, float minLimit = 0.0f,
		float step = 0.01f, float fastStep = 0.1f);
	void renderMinMaxVector(const std::string& name, glm::vec3* min, glm::vec3* max, const glm::vec3& minLimit = 
		glm::vec3{std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()});

	void renderTextureSelect(const std::string& text, const Texture* texture, std::function<void(Texture*)> setTextureCallback);
	void renderSeparatorText(const std::string& text, const ImVec4& color = ImVec4(1, 1, 1, 1), bool extraFlashy = false);

public:
	ParticleSystemEditor();
	~ParticleSystemEditor();

	void render();

	bool isEditorWindowFocused() const { return hasFocus; }
	bool isEditorWindowHovered() const { return isHovered; }

	ImGuiContext& getContext() const { return context; }
	ImGuiIO& getIO() const { return io; }
};

