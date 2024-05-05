#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "GeneralUtility/BasicMath.h"
#include "GeneralUtility/MathConstants.h"
#include "Particle System/ParticleSystem.h"
#include "ParticleSystemEditor.h"

namespace imgui = ImGui;

ParticleSystemEditor::ParticleSystemEditor() : io(imgui::GetIO()), context(*imgui::GetCurrentContext()) {}

void ParticleSystemEditor::addParticleSystem(ParticleSystem& ps)
{
	particleSystems.insert(&ps);
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

				imgui::SetNextItemWidth(100);
				if (imgui::InputFloat("Min Size", &ps->props.minSize))
				{
					float temp = utility::math::max<float>(ps->props.minSize, 0.0f);
					ps->props.minSize = utility::math::min<float>(temp, ps->props.maxSize);
				}

				imgui::SameLine(0, 30);
				imgui::SetNextItemWidth(100);

				if (imgui::InputFloat("Max Size", &ps->props.maxSize))
				{
					float temp = utility::math::max<float>(ps->props.maxSize, 0.0f);
					ps->props.maxSize = utility::math::max<float>(temp, ps->props.minSize);
				}

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

				imgui::SliderAngle("Start Rotation", &ps->props.startRotation, 0.0f, 360.0f);

				/*if (imgui::InputFloat("Start Rotation", &rot))
				{
					rot = utility::math::max<float>(rot, 0.0f);
					ps->props.startRotation = utility::math::min<float>(rot, 360.0f) * TO_RADIANS;
				}*/

				if (imgui::TreeNode("Start Color"))
				{
					imgui::ColorEdit3("Ambient", &ps->props.startColor.ambient[0]);
					imgui::ColorEdit3("Diffuse", &ps->props.startColor.diffuse[0]);
					imgui::ColorEdit3("Specular", &ps->props.startColor.specular[0]);

					imgui::SliderFloat("Alpha", &ps->props.startColor.alpha, 0.0f, 1.0f);

					imgui::TreePop();
				}

				imgui::EndTabItem();
			}
		}

		imgui::EndTabBar();
	}
}