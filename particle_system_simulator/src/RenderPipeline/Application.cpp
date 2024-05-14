
#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <stdint.h>

#include <glm/glm.hpp>
#include <glm/ext/quaternion_float.hpp>

#include "UserInterface/ParticleSystemEditor.h"
#include "ResourceManagement/ResourceManager.h"
#include "UserInterface/Gui.h"
#include "GeneralUtility/gl2fw3.h"
#include "UserInterface/Window.h"
#include "RenderPipeline/Light/PointLight.h"
#include "RenderPipeline/Light/DirectionalLight.h"
#include "RenderPipeline/Light/SpotLight.h"
#include "RenderPipeline/Transform/Transform.h"
#include "RenderPipeline/Mesh/Mesh.h"
#include "RenderPipeline/Shader/Shader.h"
#include "RenderPipeline/Texture/Texture.h"
#include "RenderPipeline/Material/Material.h"
#include "RenderPipeline/Object/MeshRenderer.h"
#include "RenderPipeline/Shader/ShaderManagement/GlobalShaderManager.h"
#include "RenderPipeline/Scene/Scene.h"
#include "MeshConstruction/Shapes.h"
#include "RenderPipeline/Application.h"
#include "Input Management/Input.h"
#include "Time Management/Time.h"
#include "Particle System/ParticleSystem.h"
#include "GeneralUtility/Random.h"
#include "Particle System/Emitter/SphereEmitter.h"
#include "Particle System/Emitter/ConeEmitter.h"
#include "Particle System/Components/Component.h"
#include "Particle System/Components/ColorBySpeed.h"
#include "Particle System/Components/ColorOverLifetime.h"
#include "Particle System/Components/ForceOverLifetime.h"
#include "Particle System/Components/LimitVelocityOverLifetime.h"
#include "Particle System/Components/RotationBySpeed.h"
#include "Particle System/Components/SizeBySpeed.h"
#include "Particle System/Components/SizeOverLifetime.h"
#include "Particle System/Components/VelocityOverLifetime.h"

Application::Application() : window(1920, 1080, "Particle Engine"), scene(1920, 1080), particleSystems(), editor()
{
	Texture texSmoke("Resources/Textures/smoke.png",
		GL_TEXTURE_2D, GL_REPEAT, GL_REPEAT, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_RGBA, GL_RGBA, ',');

	Texture texFire("Resources/Textures/fire.png",
		GL_TEXTURE_2D, GL_REPEAT, GL_REPEAT, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_RGBA, GL_RGBA, ',');

	ResourceManager::addTexture(std::move(texSmoke), "smoke");
	ResourceManager::addTexture(std::move(texFire), "fire");

	scene.createDirectionalLight(glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec3{1.0f, 1.0f, 1.0f});
}

Application& Application::getInstance()
{
	static Application application;
	return application;
}

ParticleSystem* Application::getParticleSystem(const std::string& name)
{
	auto it = std::find_if(particleSystems.begin(), particleSystems.end(), [&](const ParticleSystem& ps)
		{
			return ps.getName() == name;
		});

	if (it == particleSystems.end())
	{
		return nullptr;
	}

	return const_cast<ParticleSystem*>(&(*it));
}

void Application::run()
{
	uint32_t polygonModes[2] = {GL_FILL, GL_LINE};
	void (*glToggle[2])(GLenum) = {&glEnable, &glDisable};
	bool currentMode = 0;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Time::start();

	while (!window.shouldClose())
	{
		glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
		glClear(clearMask);
		window.pollEvents();
		Gui::beginFrame();

		if (Input::getKeyDown(KeyCode::KEY_X))
		{
			currentMode = !currentMode;
			glPolygonMode(GL_FRONT_AND_BACK, polygonModes[currentMode]);
			glToggle[currentMode](GL_CULL_FACE);
		}
		scene.update();

		for (auto& ps : particleSystems)
		{
			const_cast<ParticleSystem&>(ps).update();
		}

		scene.render();
		editor.render();

		window.swapBuffers();
		window.endFrame();
		Gui::endFrame();
		Time::endFrame();
	}
}