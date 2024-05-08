#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
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
using namespace std::string_literals;

ParticleSystemEditor::ParticleSystemEditor() : io(imgui::GetIO()), context(*imgui::GetCurrentContext()) {}

void ParticleSystemEditor::addParticleSystem(ParticleSystem& ps)
{
	particleSystems.insert(&ps);
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

void ParticleSystemEditor::renderBezierVector(const std::string& name, CubicBezierCurve<glm::vec3>* bezier, CubicBezierCurve<glm::vec3>* copy)
{
	if (imgui::TreeNodeEx(name.c_str()))
	{
		imgui::InputFloat3("Point 0", &bezier->p0[0]);
		imgui::InputFloat3("Point 1", &bezier->p1[0]);
		imgui::InputFloat3("Point 2", &bezier->p2[0]);
		imgui::InputFloat3("Point 3", &bezier->p3[0]);

		if (copy != nullptr)
		{
			copy->p0 = bezier->p0;
			copy->p1 = bezier->p1;
			copy->p2 = bezier->p2;
			copy->p3 = bezier->p3;
		}

		imgui::TreePop();
	}
}

void ParticleSystemEditor::renderBezierFloat(const std::string& name, CubicBezierCurve<float>* bezier, CubicBezierCurve<float>* copy)
{
	if (imgui::TreeNodeEx(name.c_str()))
	{
		imgui::InputFloat("Point 0", &bezier->p0);
		imgui::InputFloat("Point 1", &bezier->p1);
		imgui::InputFloat("Point 2", &bezier->p2);
		imgui::InputFloat("Point 3", &bezier->p3);

		if (copy != nullptr)
		{
			copy->p0 = bezier->p0;
			copy->p1 = bezier->p1;
			copy->p2 = bezier->p2;
			copy->p3 = bezier->p3;
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

int ParticleSystemEditor::getEmitterType(const Emitter* emitter) const
{
	if (dynamic_cast<const SphereEmitter*>(emitter))
	{
		return 0;
	}
	else if (dynamic_cast<const ConeEmitter*>(emitter))
	{
		return 1;
	}
	else
	{
		throw 100;
	}
}

void ParticleSystemEditor::render()
{
	bool isOpen;

	if (imgui::Begin("Particle System Editor", &isOpen))
	{
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
		for (ParticleSystem* ps : particleSystems)
		{
			if (imgui::BeginTabItem(ps->name.c_str()))
			{
				imgui::Text("Active Particles: %d", ps->props.currentParticles);

				addVerticalSpace(1, false);

				int mp = ps->props.maxParticles;
				if (imgui::InputInt("Max Particles", &mp))
				{
					ps->props.maxParticles = utility::math::max<int>(mp, 0);
				}

				imgui::InputFloat3("Position", &ps->props.position[0]);

				renderMinMaxFloat("Size", &ps->props.minSize, &ps->props.maxSize);

				if (imgui::InputFloat("Gravity Modifier", &ps->props.gravityModifier))
				{
					ps->props.gravityModifier = utility::math::max<float>(ps->props.gravityModifier, 0.0f);
				}

				if (imgui::InputFloat("Start Lifetime", &ps->props.startLifetime))
				{
					ps->props.startLifetime = utility::math::max<float>(ps->props.startLifetime, 0.0f);
				}

				if (imgui::InputFloat("Start Speed", &ps->props.startSpeed))
				{
					ps->props.startSpeed = utility::math::max<float>(ps->props.startSpeed, 0.0f);
				}

				if (imgui::InputFloat("Start Size", &ps->props.startSize))
				{
					ps->props.startSize = utility::math::max<float>(ps->props.startSize, 0.0f);
				}

				float rot = ps->props.startRotation * TO_DEGREES;

				if (imgui::SliderAngle("Start Rotation", &rot, 0.0f, 360.0f))
				{
					ps->props.startRotation = rot * TO_RADIANS;
				}

				renderColor("Start Color", &ps->props.startColor, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Bullet);

				renderEmitter(ps);
				renderComponents(ps);

				imgui::EndTabItem();
			}
		}

		imgui::EndTabBar();
	}
}

void ParticleSystemEditor::renderEmitter(ParticleSystem* ps)
{
	addVerticalSpace(2, false);

	if (imgui::TreeNodeEx("Emitter", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Bullet))
	{
		int prevSelected, selected;
		prevSelected = selected = getEmitterType(ps->emitter.get());

		if (imgui::Combo("Emitter Type", &selected, "Sphere\0Cone\0\0"))
		{
			if (selected != prevSelected)
			{
				switch (selected)
				{
				case 0:
					ps->emitter = std::move(SphereEmitter::defaultEmitter());
					break;
				case 1:
					ps->emitter = std::move(ConeEmitter::defaultEmitter());
					break;
				default:
					throw 100;
				}
			}
		}

		if (imgui::InputFloat("Emission Rate", &ps->emitter->emissionRate))
		{
			ps->emitter->emissionRate = utility::math::max<float>(ps->emitter->emissionRate, 0.0f);
		}

		switch (selected)
		{
		case 0:
		{
			SphereEmitter* emitter = dynamic_cast<SphereEmitter*>(ps->emitter.get());

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
			ConeEmitter* emitter = dynamic_cast<ConeEmitter*>(ps->emitter.get());

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

		imgui::TreePop();
	}
}
#include <iostream>

void ParticleSystemEditor::renderComponents(ParticleSystem* ps)
{
	addVerticalSpace(2, false);

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

		for (size_t i = 0; i < ps->components.size(); ++i)
		{
			imgui::PushID(i);

			Component* component = ps->components[i];
			ComponentType type = component->getType();
			allTypes.erase(std::remove(allTypes.begin(), allTypes.end(), type));

			bool openComponent = imgui::TreeNodeEx(getComponentTypeName(type).c_str(), ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_AllowOverlap);

			imgui::SameLine();

			imgui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_FrameRounding, 8.0f);
			imgui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));

			ImVec2 buttonSize{15.0f, 15.0f};
			ImVec2 windowSize = imgui::GetWindowSize();
			imgui::SetCursorPosX(windowSize.x * 0.9f - buttonSize.x / 2);

			if(imgui::Button("", buttonSize))
			{
				ps->removeComponent(component);
			}

			imgui::PopStyleColor();
			imgui::PopStyleVar();

			if (openComponent)
			{
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
						renderBezierVector("Bezier Curve", &vol->minBezier, &vol->maxBezier);
					}
					break;
					case 2:
					{
						renderMinMaxVector("Velocity", &vol->minVelocity, &vol->maxVelocity);
					}
					break;
					case 3:
					{
						renderBezierVector("Lower Bezier Curve", &vol->minBezier);
						renderBezierVector("Upper Bezier Curve", &vol->maxBezier);
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
						renderBezierFloat("Bezier Curve", &sol->minBezier, &sol->maxBezier);
					}
					break;
					case 1:
					{
						renderMinMaxFloat("Size", &sol->minSize, &sol->maxSize);
					}
					break;
					case 2:
					{
						renderBezierFloat("Lower Bezier Curve", &sol->minBezier);
						renderBezierFloat("Upper Bezier Curve", &sol->maxBezier);
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
						renderBezierFloat("Bezier Curve", &sbs->minBezier, &sbs->maxBezier);
					}
					break;
					case 1:
					{
						renderBezierFloat("Lower Bezier Curve", &sbs->minBezier);
						renderBezierFloat("Upper Bezier Curve", &sbs->maxBezier);
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
						renderBezierFloat("Bezier Curve", &rbs->minBezier, &rbs->maxBezier);
					}
					break;
					case 1:
					{
						renderBezierFloat("Lower Bezier Curve", &rbs->minBezier);
						renderBezierFloat("Upper Bezier Curve", &rbs->maxBezier);
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
						renderBezierVector("Bezier Curve", &lvol->minBezier, &lvol->maxBezier);
					}
					break;
					case 2:
					{
						renderMinMaxVector("Velocity", &lvol->minVelocity, &lvol->maxVelocity);
					}
					break;
					case 3:
					{
						renderBezierVector("Lower Bezier Curve", &lvol->minBezier);
						renderBezierVector("Upper Bezier Curve", &lvol->maxBezier);
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
						renderBezierVector("Bezier Curve", &fol->minBezier, &fol->maxBezier);
					}
					break;
					case 2:
					{
						renderMinMaxVector("Velocity", &fol->minForce, &fol->maxForce);
					}
					break;
					case 3:
					{
						renderBezierVector("Lower Bezier Curve", &fol->minBezier);
						renderBezierVector("Upper Bezier Curve", &fol->maxBezier);
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
					renderColor("Color-1", &col->colors[0]);
					renderColor("Color-2", &col->colors[1]);
				}
				break;
				case ComponentType::Color_By_Speed:
				{
					ColorBySpeed* cbs = dynamic_cast<ColorBySpeed*>(component);
					renderMinMaxFloat("Speed", &cbs->minSpeed, &cbs->maxSpeed);
					renderMinMaxFloat("Keypoint-1", "Keypoint-2", &cbs->keypoints[0], &cbs->keypoints[1]);
					renderColor("Color-1", &cbs->colors[0]);
					renderColor("Color-2", &cbs->colors[1]);
				}
				break;
				default: throw 100;
				}

				imgui::TreePop();
			}

			imgui::PopID();
		}

		if (allTypes.size() > 0)
		{
			addVerticalSpace(2, false);

			ImVec2 windowSize = imgui::GetWindowSize();
			ImVec2 buttonSize{windowSize.x * 0.64f, 25.0f};
			imgui::SetCursorPosX(windowSize.x / 2 - buttonSize.x / 2);

			if (imgui::Button("Add Component", buttonSize))
			{
				imgui::OpenPopup("Components");
			}

			if (imgui::BeginPopup("Components", ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysUseWindowPadding))
			{
				for (size_t i = 0; i < allTypes.size(); ++i)
				{
					if (imgui::Selectable(getComponentTypeName(allTypes[i]).c_str()))
					{
						switch (allTypes[i])
						{
						case ComponentType::Velocity_Over_Lifetime: ps->addComponent(new VelocityOverLifetime()); break;
						case ComponentType::Size_Over_Lifetime: ps->addComponent(new SizeOverLifetime()); break;
						case ComponentType::Size_By_Speed: ps->addComponent(new SizeBySpeed()); break;
						case ComponentType::Rotation_By_Speed: ps->addComponent(new RotationBySpeed()); break;
						case ComponentType::Limit_Velocity_Over_Lifetime: ps->addComponent(new LimitVelocityOverLifetime()); break;
						case ComponentType::Force_Over_Lifetime: ps->addComponent(new ForceOverLifetime()); break;
						case ComponentType::Color_Over_Lifetime: ps->addComponent(new ColorOverLifetime()); break;
						case ComponentType::Color_By_Speed: ps->addComponent(new ColorBySpeed()); break;
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