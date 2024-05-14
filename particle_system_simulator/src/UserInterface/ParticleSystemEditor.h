#pragma once

#include <set>
#include <stdint.h>
#include <limits>
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
	void renderEmitter(ParticleSystem& ps);
	void renderComponents(ParticleSystem& ps);

	void renderBezierVector(const std::string& name, const std::string& yAxis, BezierCurve<glm::vec3>* bezier, BezierCurve<glm::vec3>* copy = nullptr, ImGuiTreeNodeFlags flags = 0);
	void renderBezierFloat(const std::string& name, const std::string& yAxis, BezierCurve<float>* bezier, BezierCurve<float>* copy = nullptr,
		ImGuiTreeNodeFlags flags = 0);

	void renderColor(const std::string& name, Color4* color, ImGuiTreeNodeFlags flags = 0);

	void renderMinMaxFloat(const std::string& name, float* min, float* max, float minLimit = 0.0f);
	void renderMinMaxFloat(const std::string& minName, const std::string& maxName, float* min, float* max, float minLimit = 0.0f);
	void renderMinMaxVector(const std::string& name, glm::vec3* min, glm::vec3* max, const glm::vec3& minLimit = 
		glm::vec3{std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()});
	void renderTextureSelect(const std::string& text, Texture** texture);
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

