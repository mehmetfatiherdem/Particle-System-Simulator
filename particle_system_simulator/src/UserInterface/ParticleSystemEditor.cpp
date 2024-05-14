#include <iostream>
#include <map>
#include <set>
#include <optional>
#include "RenderPipeline/Application.h"
#include "ResourceManagement/ResourceManager.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/implot.h"
#include "GeneralUtility/BasicMath.h"
#include "GeneralUtility/MathConstants.h"
#include "Particle System/ParticleSystem.h"
#include "ParticleSystemEditor.h"
#include <Particle System/Emitter/SphereEmitter.h>
#include <Particle System/Emitter/ConeEmitter.h>
#include "Particle System/Components/Component.h"
#include "Particle System/Components/ColorBySpeed.h"
#include "Particle System/Components/ColorOverLifetime.h"
#include "Particle System/Components/ForceOverLifetime.h"
#include "Particle System/Components/LimitVelocityOverLifetime.h"
#include "Particle System/Components/RotationBySpeed.h"
#include "Particle System/Components/SizeBySpeed.h"
#include "Particle System/Components/SizeOverLifetime.h"
#include "Particle System/Components/VelocityOverLifetime.h"

namespace imgui = ImGui;
namespace implot = ImPlot;
using namespace std::string_literals;

ParticleSystemEditor::ParticleSystemEditor() : io(imgui::GetIO()), context(*imgui::GetCurrentContext()), emptyTexture(nullptr),
hasFocus(true), isHovered(false)
{
	uint32_t width, height;
	width = height = 4;
	unsigned char* pixels = new unsigned char[width * height * 3];

	for (size_t i = 0; i < width * height; ++i)
	{
		pixels[i * 3] = 255;
		pixels[i * 3 + 1] = 255;
		pixels[i * 3 + 2] = 255;
	}

	emptyTexture = new Texture(pixels, width, height, GL_TEXTURE_2D, GL_REPEAT, GL_REPEAT, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_RGB, GL_RGB);

	delete[] pixels;
}

ParticleSystemEditor::~ParticleSystemEditor()
{
	delete emptyTexture;
}

void ParticleSystemEditor::addVerticalSpace(uint32_t count, bool useLargeSpaces)
{
	for (size_t i = 0; i < count; ++i)
	{
		if (useLargeSpaces)
		{
			imgui::NewLine();
		}
		else
		{
			imgui::Spacing();
		}
	}
}

void ParticleSystemEditor::renderBezierVector(const std::string& name, const std::string& yAxis, BezierCurve<glm::vec3>* bezier, BezierCurve<glm::vec3>* copy, ImGuiTreeNodeFlags flags)
{
	if (imgui::TreeNodeEx(name.c_str(), flags))
	{
		if (imgui::TreeNodeEx("Control Points"))
		{
			ImGuiStyle& style = imgui::GetStyle();
			ImVec4 defaultButtonColor = style.Colors[ImGuiCol_Button];

			if (imgui::Button("Add In Front"))
			{
				bezier->addControlPoint(-1, glm::vec3{0.0f, 0.0f, 0.0f});
			}

			for (size_t i = 0; i < bezier->size(); ++i)
			{
				style.Colors[ImGuiCol_Button] = defaultButtonColor;
				imgui::InputFloat3(("Point " + std::to_string(i)).c_str(), &(*bezier)[i][0]);

				if (bezier->size() < 25)
				{
					style.Colors[ImGuiCol_Button] = ImVec4(0.3f, 1.0f, 0.3f, 1.0f);
					imgui::SameLine(0.0f, 5.0f);
					imgui::SetCursorPosX(windowSize.x * 0.9f);
					imgui::Button("+", ImVec2(20.0f, 20.0f));

					if (imgui::IsItemClicked())
					{
						bezier->addControlPoint(i, glm::vec3{0.0f, 0.0f, 0.0f});
					}
				}

				if (bezier->size() > 2)
				{
					style.Colors[ImGuiCol_Button] = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
					imgui::SameLine(0.0f, 5.0f);

					if (bezier->size() >= 25)
					{
						imgui::SetCursorPosX(windowSize.x * 0.9f + 25.0f);
					}

					imgui::Button("-", ImVec2(20.0f, 20.0f));

					if (imgui::IsItemClicked())
					{
						bezier->deleteControlPoint(i);
					}
				}
			}

			style.Colors[ImGuiCol_Button] = defaultButtonColor;

			if (copy != nullptr)
			{
				*copy = *bezier;
			}

			imgui::TreePop();
		}

		auto [time, values] = bezier->evaluateCurve(50);

		std::vector<float> x, y, z;
		x.reserve(values.size());
		y.reserve(values.size());
		z.reserve(values.size());

		std::for_each(values.begin(), values.end(), [&x, &y, &z](const glm::vec3& vec)
			{
				x.push_back(vec.x);
				y.push_back(vec.y);
				z.push_back(vec.z);
			});

		implot::SetNextAxesToFit();
		if (implot::BeginPlot("Bezier"))
		{
			implot::SetupAxes("time", yAxis.c_str());
			implot::SetNextLineStyle(ImVec4(1.0f, 0.1f, 0.2f, 1.0f));
			implot::PlotLine("x", time.data(), x.data(), values.size());
			implot::SetNextLineStyle(ImVec4(0.2f, 1.0f, 0.1f, 1.0f));
			implot::PlotLine("y", time.data(), y.data(), values.size());
			implot::SetNextLineStyle(ImVec4(0.1f, 0.2f, 1.0f, 1.0f));
			implot::PlotLine("z", time.data(), z.data(), values.size());
			implot::EndPlot();
		}

		imgui::TreePop();
	}
}

void ParticleSystemEditor::renderBezierFloat(const std::string& name, const std::string& yAxis, BezierCurve<float>* bezier, BezierCurve<float>* copy, ImGuiTreeNodeFlags flags)
{
	if (imgui::TreeNodeEx(name.c_str(), flags))
	{
		if (imgui::TreeNodeEx("Control Points"))
		{
			ImGuiStyle& style = imgui::GetStyle();
			ImVec4 defaultButtonColor = style.Colors[ImGuiCol_Button];

			if (imgui::Button("Add In Front"))
			{
				bezier->addControlPoint(-1, 0.0f);
			}

			for (size_t i = 0; i < bezier->size(); ++i)
			{
				style.Colors[ImGuiCol_Button] = defaultButtonColor;
				imgui::InputFloat(("Point " + std::to_string(i)).c_str(), &(*bezier)[i], 0.01f, 0.1f);

				if (bezier->size() < 25)
				{
					style.Colors[ImGuiCol_Button] = ImVec4(0.3f, 1.0f, 0.3f, 1.0f);
					imgui::SameLine(0.0f, 5.0f);
					imgui::SetCursorPosX(windowSize.x * 0.9f);
					imgui::Button("+", ImVec2(20.0f, 20.0f));

					if (imgui::IsItemClicked())
					{
						bezier->addControlPoint(i, 0.0f);
					}
				}

				if (bezier->size() > 2)
				{
					style.Colors[ImGuiCol_Button] = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
					imgui::SameLine(0.0f, 5.0f);

					if (bezier->size() >= 25)
					{
						imgui::SetCursorPosX(windowSize.x * 0.9f + 25.0f);
					}

					imgui::Button("-", ImVec2(20.0f, 20.0f));

					if (imgui::IsItemClicked())
					{
						bezier->deleteControlPoint(i);
					}
				}
			}

			style.Colors[ImGuiCol_Button] = defaultButtonColor;

			if (copy != nullptr)
			{
				*copy = *bezier;
			}

			imgui::TreePop();
		}

		auto [time, values] = bezier->evaluateCurve(50);

		implot::SetNextAxesToFit();
		if (implot::BeginPlot("Bezier"))
		{
			//implot::SetupAxisZoomConstraints(ImAxis_::ImAxis_X1, 1.0f, 1.0f);
			//implot::SetupAxisZoomConstraints(ImAxis_::ImAxis_Y1, 1.0f, 1.0f);

			implot::SetupAxes("time", yAxis.c_str());
			implot::PlotLine("s", time.data(), values.data(), values.size());
			implot::EndPlot();
		}

		imgui::TreePop();
	}
}

void ParticleSystemEditor::renderColor(const std::string& name, Color4* color, ImGuiTreeNodeFlags flags)
{
	if (imgui::TreeNodeEx(name.c_str(), flags))
	{
		imgui::ColorEdit3("Ambient", &color->ambient[0]);
		imgui::ColorEdit3("Diffuse", &color->diffuse[0]);
		imgui::ColorEdit3("Specular", &color->specular[0]);

		imgui::SliderFloat("Alpha", &color->alpha, 0.0f, 1.0f);

		imgui::TreePop();
	}
}

void ParticleSystemEditor::renderMinMaxFloat(const std::string& name, float* min, float* max, float minLimit)
{
	if (imgui::InputFloat(("Min "s.append(name).c_str()), min))
	{
		*min = utility::math::max<float>(*min, minLimit);
		*min = utility::math::min<float>(*max, *min);
	}

	if (imgui::InputFloat("Max "s.append(name).c_str(), max))
	{
		*max = utility::math::max<float>(*max, *min);
	}
}

void ParticleSystemEditor::renderMinMaxVector(const std::string& name, glm::vec3* min, glm::vec3* max, const glm::vec3& minLimit)
{
	if (imgui::InputFloat3(("Min "s.append(name).c_str()), &((*min)[0])))
	{
		min->x = utility::math::max<float>(min->x, minLimit.x);
		min->x = utility::math::min<float>(max->x, min->x);
		min->y = utility::math::max<float>(min->y, minLimit.y);
		min->y = utility::math::min<float>(max->y, min->y);
		min->z = utility::math::max<float>(min->z, minLimit.z);
		min->z = utility::math::min<float>(max->z, min->z);
	}

	if (imgui::InputFloat3(("Max "s.append(name).c_str()), &((*max)[0])))
	{
		max->x = utility::math::max<float>(max->x, min->x);
		max->y = utility::math::max<float>(max->y, min->y);
		max->z = utility::math::max<float>(max->z, min->z);
	}
}

void ParticleSystemEditor::renderMinMaxFloat(const std::string& minName, const std::string& maxName, float* min, float* max, float minLimit)
{
	if (imgui::InputFloat(minName.c_str(), min))
	{
		*min = utility::math::max<float>(*min, minLimit);
		*min = utility::math::min<float>(*max, *min);
	}

	if (imgui::InputFloat(maxName.c_str(), max))
	{
		*max = utility::math::max<float>(*max, *min);
	}
}

void ParticleSystemEditor::renderTextureSelect(const std::string& text, Texture** texture)
{
	const float framePadding = 1.0f;
	const ImVec2 imageSize = ImVec2(30.0f, 30.0f);
	const float imagePosY = imgui::GetCursorPosY();
	const float textPosY = imagePosY + framePadding + imageSize.y / 2 - 2.0f;
	const std::string popupName = "Textures" + text;
	void* textureId = (void*)(intptr_t)(*texture == nullptr ? emptyTexture->getTextureID() : (*texture)->getTextureID());

	const float popupImageSpacing = 7.0f;
	const float popupImagePadding = 10.0f;
	const ImVec2 defaultSpacing = imgui::GetStyle().ItemSpacing;
	const ImVec2 defaultPadding = imgui::GetStyle().WindowPadding;

	imgui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_FramePadding, ImVec2(framePadding, framePadding));

	imgui::SetCursorPosY(textPosY);
	imgui::Text(text.c_str());

	imgui::SameLine();
	imgui::SetCursorPos(ImVec2(windowSize.x * 0.75f - imageSize.x / 2, imagePosY));

	imgui::Image(textureId, imageSize, ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1.0f, 0.2f, 0.3f, 1.0f));
	imgui::PopStyleVar();

	imgui::SameLine();
	imgui::SetCursorPosY((imagePosY + textPosY) / 2.0f);

	imgui::ArrowButton("", ImGuiDir_::ImGuiDir_Down);

	if (imgui::IsItemClicked())
	{
		imgui::OpenPopup(popupName.c_str());
	}

	if (imgui::BeginPopup(popupName.c_str(), ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar))
	{
		ImVec2 cursorPos = imgui::GetCursorPos();
		imgui::SetCursorPos(ImVec2(cursorPos.x + popupImagePadding - defaultPadding.x, cursorPos.y + popupImagePadding - defaultPadding.y));

		if (imgui::ImageButton((void*)(intptr_t)emptyTexture->getTextureID(), imageSize, ImVec2(0, 0), ImVec2(1, 1), -1,
			ImVec4(0, 0, 0, 1), ImVec4(1, 1, 1, 1)))
		{
			*texture = nullptr;
			imgui::CloseCurrentPopup();
		}

		if (imgui::IsItemHovered())
		{
			imgui::SetTooltip("None");
		}

		uint32_t index = 1;

		for (auto it = ResourceManager::texturesBegin(); it != ResourceManager::texturesEnd(); ++it, ++index)
		{
			if (index % 5 != 0)
			{
				imgui::SameLine(0.0f, popupImageSpacing);
			}
			else
			{
				cursorPos = imgui::GetCursorPos();
				imgui::SetCursorPos(ImVec2(cursorPos.x + popupImagePadding - defaultPadding.x, cursorPos.y + popupImageSpacing - defaultSpacing.y));
			}

			if (imgui::ImageButton((void*)(intptr_t)it->second.getTextureID(), imageSize, ImVec2(0, 0), ImVec2(1, 1), -1,
				ImVec4(0, 0, 0, 1), ImVec4(1, 1, 1, 1)))
			{
				*texture = &it->second;
				imgui::CloseCurrentPopup();
			}

			if (imgui::IsItemHovered())
			{
				imgui::SetTooltip(it->first.c_str());
			}
		}

		imgui::EndPopup();
	}
}

void ParticleSystemEditor::renderSeparatorText(const std::string& text, const ImVec4& color, bool extraFlashy)
{
	imgui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, color);

	if (extraFlashy)
	{
		imgui::Separator();
	}

	imgui::SeparatorText(text.c_str());

	if (extraFlashy)
	{
		imgui::Separator();
	}

	imgui::PopStyleColor();
}

void ParticleSystemEditor::render()
{
	if (imgui::Begin("Particle System Editor"))
	{
		windowSize = imgui::GetWindowSize();
		hasFocus = imgui::IsWindowFocused();
		isHovered = imgui::IsWindowHovered();

		renderParticleTabs();
		imgui::End();
	}

	imgui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(imgui::GetDrawData());
}

void ParticleSystemEditor::renderParticleTabs()
{
	if (imgui::BeginTabBar("Particle Systems"))
	{
		ParticleSystem* selectedForRemoval = nullptr;

		for (auto& temp : Application::getInstance().getParticleSystems())
		{
			ParticleSystem& ps = const_cast<ParticleSystem&>(temp);

			bool tab = imgui::BeginTabItem(ps.name.c_str());

			if (imgui::IsMouseClicked(1) && imgui::IsItemHovered())
			{
				selectedForRemoval = &ps;
			}

			if (tab)
			{
				imgui::Checkbox("Enabled", &ps.enabled);

				imgui::BeginDisabled(!ps.enabled);
				imgui::Text("Active Particles: %d", ps.props.currentParticles);

				char buffer[50];
				ps.name.copy(buffer, sizeof(buffer) - 1);
				uint32_t size = ps.name.size() > 49 ? 49 : ps.name.size();
				buffer[size] = '\0';

				addVerticalSpace(1, false);
				renderSeparatorText("Properties", ImVec4(0.67f, 0.34f, 0.72f, 1.0f), true);
				addVerticalSpace(1, false);

				if (imgui::InputText("Name", buffer, sizeof(buffer), ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue))
				{
					std::memcpy(ps.name.data(), buffer, size);
					ps.name = buffer;
				}

				int mp = ps.props.maxParticles;

				if (imgui::InputInt("Max Particles", &mp))
				{
					ps.setMaxParticles(utility::math::max<uint32_t>(mp, 0));
				}

				imgui::InputFloat3("Position", &ps.props.position[0]);

				renderMinMaxFloat("Size", &ps.props.minSize, &ps.props.maxSize);

				if (imgui::InputFloat("Gravity Modifier", &ps.props.gravityModifier))
				{
					ps.props.gravityModifier = utility::math::max<float>(ps.props.gravityModifier, 0.0f);
				}

				if (imgui::InputFloat("Start Lifetime", &ps.props.startLifetime))
				{
					ps.props.startLifetime = utility::math::max<float>(ps.props.startLifetime, 0.0f);
				}

				if (imgui::InputFloat("Start Speed", &ps.props.startSpeed))
				{
					ps.props.startSpeed = utility::math::max<float>(ps.props.startSpeed, 0.0f);
				}

				if (imgui::InputFloat("Start Size", &ps.props.startSize))
				{
					ps.props.startSize = utility::math::max<float>(ps.props.startSize, 0.0f);
				}

				float rot = ps.props.startRotation * TO_DEGREES;

				if (imgui::SliderAngle("Start Rotation", &rot, 0.0f, 360.0f))
				{
					ps.props.startRotation = rot * TO_RADIANS;
				}

				renderSeparatorText("Material", ImVec4(0.67f, 0.34f, 0.72f, 1.0f), false);

				renderTextureSelect("Diffuse Map", &ps.material.diffuseMap);
				addVerticalSpace(1, false);
				renderTextureSelect("Specular Map", &ps.material.specularMap);

				addVerticalSpace(2, false);

				float shininess = ps.material.getShininess();

				if (imgui::InputFloat("Shininess", &shininess))
				{
					ps.material.setShininess(utility::math::max<float>(0.0f, shininess));
				}

				renderColor("Start Color", &ps.props.startColor, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen);

				renderEmitter(ps);
				renderComponents(ps);

				imgui::EndDisabled();
				imgui::EndTabItem();
			}
		}

		if (selectedForRemoval != nullptr)
		{
			Application::getInstance().removeParticleSystem(*selectedForRemoval);
		}

		if (imgui::TabItemButton("+", ImGuiTabItemFlags_::ImGuiTabItemFlags_Trailing))
		{
			auto emitter = SphereEmitter::defaultEmitter();

			std::string name = "New Particle System";
			ParticleSystem* existing = Application::getInstance().getParticleSystem(name);

			for (size_t nr = 1; existing != nullptr; ++nr)
			{
				name = "New Particle System(" + std::to_string(nr) + ")";
				existing = Application::getInstance().getParticleSystem(name);
			}

			ParticleSystem ps(std::move(name), ParticleSystemProps(), Material::defaultMaterial(), std::move(emitter));
			Application::getInstance().addParticleSystem(std::move(ps));
		}

		imgui::EndTabBar();
	}
}

void ParticleSystemEditor::renderEmitter(ParticleSystem& ps)
{
	renderSeparatorText("Emitter", ImVec4(0.67f, 0.34f, 0.72f, 1.0f), false);

	int prevSelected, selected;
	prevSelected = selected = static_cast<int>(ps.emitter.get()->getType());

	if (imgui::Combo("Emitter Type", &selected, "Sphere\0Cone\0\0"))
	{
		if (selected != prevSelected)
		{
			switch (static_cast<EmitterType>(selected))
			{
			case EmitterType::SphereEmitter:
				ps.emitter = std::move(SphereEmitter::defaultEmitter());
				break;
			case EmitterType::ConeEmitter:
				ps.emitter = std::move(ConeEmitter::defaultEmitter());
				break;
			default:
				throw 100;
			}
		}
	}

	if (imgui::InputFloat("Emission Rate", &ps.emitter->emissionRate))
	{
		ps.emitter->emissionRate = utility::math::max<float>(ps.emitter->emissionRate, 0.0f);
	}

	switch (selected)
	{
	case 0:
	{
		SphereEmitter* emitter = dynamic_cast<SphereEmitter*>(ps.emitter.get());

		if (imgui::InputFloat("Radius", &emitter->radius))
		{
			emitter->radius = utility::math::max<float>(emitter->radius, 0.0f);
		}

		float rot = emitter->arc * TO_DEGREES;

		if (imgui::SliderFloat("Arc", &rot, 1.0f, 360.0f))
		{
			emitter->arc = rot * TO_RADIANS;
		}
	}
	break;
	case 1:
	{
		ConeEmitter* emitter = dynamic_cast<ConeEmitter*>(ps.emitter.get());

		if (imgui::InputFloat("Radius", &emitter->radius))
		{
			emitter->radius = utility::math::max<float>(emitter->radius, 0.0f);
		}

		float rot = emitter->angle * TO_DEGREES;

		if (imgui::SliderFloat("Arc", &rot, 0.0f, 360.0f))
		{
			emitter->angle = rot * TO_RADIANS;
		}
	}
	break;
	default: throw 100;
	}
}

void ParticleSystemEditor::renderComponents(ParticleSystem& ps)
{
	addVerticalSpace(1, false);

	if (imgui::TreeNodeEx("Components", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Framed))
	{
		std::vector<ComponentType> allTypes = {
			ComponentType::Velocity_Over_Lifetime,
			ComponentType::Size_Over_Lifetime,
			ComponentType::Size_By_Speed,
			ComponentType::Rotation_By_Speed,
			ComponentType::Limit_Velocity_Over_Lifetime,
			ComponentType::Force_Over_Lifetime,
			ComponentType::Color_Over_Lifetime,
			ComponentType::Color_By_Speed
		};

		auto iterator = ps.components.begin();

		for (uint32_t i = 0; i < ps.components.size(); ++i)
		{
			imgui::PushID(i);

			Component* component = ps.components[i];
			ComponentType type = component->getType();
			allTypes.erase(std::remove(allTypes.begin(), allTypes.end(), type));

			bool enabled = component->isEnabled();

			if (imgui::Checkbox("", &enabled))
			{
				component->toggle();
			}

			imgui::SameLine();
			imgui::BeginDisabled(!enabled);
			bool openComponent = imgui::TreeNodeEx(getComponentTypeName(type).c_str(), ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_AllowOverlap);
			imgui::EndDisabled();
			imgui::SameLine();

			imgui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_FrameRounding, 8.0f);
			imgui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));

			ImVec2 buttonSize{15.0f, 15.0f};
			imgui::SetCursorPosX(windowSize.x * 0.9f - buttonSize.x / 2);

			imgui::Button("", buttonSize);

			imgui::PopStyleColor();
			imgui::PopStyleVar();

			if (imgui::IsItemClicked(0))
			{
				ps.removeComponent(component);
				enabled = false;
			}

			if (enabled && openComponent)
			{
				imgui::BeginDisabled(!enabled);

				addVerticalSpace(2);
				switch (type)
				{
				case ComponentType::Velocity_Over_Lifetime:
				{
					VelocityOverLifetime* vol = dynamic_cast<VelocityOverLifetime*>(component);

					int selected = static_cast<int>(vol->method);

					if (imgui::Combo("Method", &selected, "Constant\0Curve\0Random Between Two Constants\0Random Between Two Curves\0\0"))
					{
						vol->method = static_cast<ComponentMethod>(selected);
					}

					switch (selected)
					{
					case 0:
					{
						if (imgui::InputFloat3("Velocity", &vol->minVelocity[0]))
						{
							vol->maxVelocity = vol->minVelocity;
						}
					}
					break;
					case 1:
					{
						renderBezierVector("Bezier Curve", "Velocity", & vol->minBezier, &vol->maxBezier, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Bullet);
					}
					break;
					case 2:
					{
						renderMinMaxVector("Velocity", &vol->minVelocity, &vol->maxVelocity);
					}
					break;
					case 3:
					{
						renderBezierVector("Lower Bezier Curve", "Velocity", & vol->minBezier, nullptr, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Bullet);
						renderBezierVector("Upper Bezier Curve", "Velocity", & vol->maxBezier, nullptr, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Bullet);
					}
					break;
					default: throw 100;
					}
				}
				break;
				case ComponentType::Size_Over_Lifetime:
				{
					SizeOverLifetime* sol = dynamic_cast<SizeOverLifetime*>(component);

					int selected = static_cast<int>(sol->method) - 1;

					if (imgui::Combo("Method", &selected, "Curve\0Random Between Two Constants\0Random Between Two Curves\0\0"))
					{
						sol->method = static_cast<ComponentMethod>(selected + 1);
					}

					switch (selected)
					{
					case 0:
					{
						renderBezierFloat("Bezier Curve", "Size", & sol->minBezier, &sol->maxBezier, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Bullet);
					}
					break;
					case 1:
					{
						renderMinMaxFloat("Size", &sol->minSize, &sol->maxSize);
					}
					break;
					case 2:
					{
						renderBezierFloat("Lower Bezier Curve", "Size", & sol->minBezier, nullptr, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Bullet);
						renderBezierFloat("Upper Bezier Curve", "Size", & sol->maxBezier, nullptr, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Bullet);
					}
					break;
					default: throw 100;
					}
				}
				break;
				case ComponentType::Size_By_Speed:
				{
					SizeBySpeed* sbs = dynamic_cast<SizeBySpeed*>(component);

					int selected = (sbs->method == ComponentMethod::Curve) ? 0 :
						(sbs->method == ComponentMethod::Random_Between_Two_Curves) ? 1 : -1;

					if (imgui::Combo("Method", &selected, "Curve\0Random Between Two Curves\0\0"))
					{
						sbs->method = static_cast<ComponentMethod>((selected == 0) ? 1 : (selected == 1) ? 3 : -1);
					}

					renderMinMaxFloat("Speed", &sbs->minSpeed, &sbs->maxSpeed);

					switch (selected)
					{
					case 0:
					{
						renderBezierFloat("Bezier Curve", "Size", & sbs->minBezier, &sbs->maxBezier, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Bullet);
					}
					break;
					case 1:
					{
						renderBezierFloat("Lower Bezier Curve", "Size", & sbs->minBezier, nullptr, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Bullet);
						renderBezierFloat("Upper Bezier Curve", "Size", & sbs->maxBezier, nullptr, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Bullet);
					}
					break;
					default: throw 100;
					}
				}
				break;
				case ComponentType::Rotation_By_Speed:
				{
					RotationBySpeed* rbs = dynamic_cast<RotationBySpeed*>(component);

					int selected = (rbs->method == ComponentMethod::Curve) ? 0 :
						(rbs->method == ComponentMethod::Random_Between_Two_Curves) ? 1 : -1;

					if (imgui::Combo("Method", &selected, "Curve\0Random Between Two Curves\0\0"))
					{
						rbs->method = static_cast<ComponentMethod>((selected == 0) ? 1 : (selected == 1) ? 3 : -1);
					}

					renderMinMaxFloat("Speed", &rbs->minSpeed, &rbs->maxSpeed);

					switch (selected)
					{
					case 0:
					{
						renderBezierFloat("Bezier Curve", "Rotation", & rbs->minBezier, &rbs->maxBezier, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Bullet);
					}
					break;
					case 1:
					{
						renderBezierFloat("Lower Bezier Curve", "Rotation", & rbs->minBezier, nullptr, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Bullet);
						renderBezierFloat("Upper Bezier Curve", "Rotation", & rbs->maxBezier, nullptr, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Bullet);
					}
					break;
					default: throw 100;
					}
				}
				break;
				case ComponentType::Limit_Velocity_Over_Lifetime:
				{
					LimitVelocityOverLifetime* lvol = dynamic_cast<LimitVelocityOverLifetime*>(component);

					int selected = static_cast<int>(lvol->method);

					if (imgui::Combo("Method", &selected, "Constant\0Curve\0Random Between Two Constants\0Random Between Two Curves\0\0"))
					{
						lvol->method = static_cast<ComponentMethod>(selected);
					}

					imgui::SliderFloat("Dampen", &lvol->dampen, 0.0f, 1.0f);

					switch (selected)
					{
					case 0:
					{
						if (imgui::InputFloat3("Velocity", &lvol->minVelocity[0]))
						{
							lvol->maxVelocity = lvol->minVelocity;
						}
					}
					break;
					case 1:
					{
						renderBezierVector("Bezier Curve", "Limit Velocity", &lvol->minBezier, &lvol->maxBezier, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Bullet);
					}
					break;
					case 2:
					{
						renderMinMaxVector("Velocity", &lvol->minVelocity, &lvol->maxVelocity);
					}
					break;
					case 3:
					{
						renderBezierVector("Lower Bezier Curve", "Limit Velocity", &lvol->minBezier, nullptr, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Bullet);
						renderBezierVector("Upper Bezier Curve", "Limit Velocity", &lvol->maxBezier, nullptr, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Bullet);
					}
					break;
					default: throw 100;
					}
				}
				break;
				case ComponentType::Force_Over_Lifetime:
				{
					ForceOverLifetime* fol = dynamic_cast<ForceOverLifetime*>(component);

					int selected = static_cast<int>(fol->method);

					if (imgui::Combo("Method", &selected, "Constant\0Curve\0Random Between Two Constants\0Random Between Two Curves\0\0"))
					{
						fol->method = static_cast<ComponentMethod>(selected);
					}

					switch (selected)
					{
					case 0:
					{
						if (imgui::InputFloat3("Velocity", &fol->minForce[0]))
						{
							fol->maxForce = fol->minForce;
						}
					}
					break;
					case 1:
					{
						renderBezierVector("Bezier Curve", "Force", &fol->minBezier, &fol->maxBezier, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Bullet);
					}
					break;
					case 2:
					{
						renderMinMaxVector("Velocity", &fol->minForce, &fol->maxForce);
					}
					break;
					case 3:
					{
						renderBezierVector("Lower Bezier Curve", "Force", &fol->minBezier, nullptr, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Bullet);
						renderBezierVector("Upper Bezier Curve", "Force", &fol->maxBezier, nullptr, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Bullet);
					}
					break;
					default: throw 100;
					}
				}
				break;
				case ComponentType::Color_Over_Lifetime:
				{
					ColorOverLifetime* col = dynamic_cast<ColorOverLifetime*>(component);
					renderMinMaxFloat("Keypoint-1", "Keypoint-2", &col->keypoints[0], &col->keypoints[1]);
					renderColor("Color-1", &col->colors[0], ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Bullet);
					renderColor("Color-2", &col->colors[1], ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Bullet);
				}
				break;
				case ComponentType::Color_By_Speed:
				{
					ColorBySpeed* cbs = dynamic_cast<ColorBySpeed*>(component);
					renderMinMaxFloat("Speed", &cbs->minSpeed, &cbs->maxSpeed);
					renderMinMaxFloat("Keypoint-1", "Keypoint-2", &cbs->keypoints[0], &cbs->keypoints[1]);
					renderColor("Color-1", &cbs->colors[0], ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Bullet);
					renderColor("Color-2", &cbs->colors[1], ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Bullet);
				}
				break;
				default: throw 100;
				}

				imgui::EndDisabled();
			}

			if (openComponent)
			{
				imgui::TreePop();
			}

			imgui::PopID();
		}

		if (allTypes.size() > 0)
		{
			addVerticalSpace(2, false);

			ImVec2 buttonSize{windowSize.x * 0.64f, 25.0f};
			imgui::SetCursorPosX(windowSize.x / 2 - buttonSize.x / 2);

			if (imgui::Button("Add Component", buttonSize))
			{
				imgui::OpenPopup("Components");
			}

			if (imgui::BeginPopup("Components", ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar))
			{
				for (size_t i = 0; i < allTypes.size(); ++i)
				{
					if (imgui::Selectable(getComponentTypeName(allTypes[i]).c_str()))
					{
						switch (allTypes[i])
						{
						case ComponentType::Velocity_Over_Lifetime: ps.addComponent(new VelocityOverLifetime()); break;
						case ComponentType::Size_Over_Lifetime: ps.addComponent(new SizeOverLifetime()); break;
						case ComponentType::Size_By_Speed: ps.addComponent(new SizeBySpeed()); break;
						case ComponentType::Rotation_By_Speed: ps.addComponent(new RotationBySpeed()); break;
						case ComponentType::Limit_Velocity_Over_Lifetime: ps.addComponent(new LimitVelocityOverLifetime()); break;
						case ComponentType::Force_Over_Lifetime: ps.addComponent(new ForceOverLifetime()); break;
						case ComponentType::Color_Over_Lifetime: ps.addComponent(new ColorOverLifetime()); break;
						case ComponentType::Color_By_Speed: ps.addComponent(new ColorBySpeed()); break;
						default: throw 100;
						}
					}
				}

				imgui::EndPopup();
			}
		}

		imgui::TreePop();
	}
}
