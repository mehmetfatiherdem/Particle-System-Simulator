
#include <iostream>
#include <cstring>
#include <cmath>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>

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

Application::Application() : window(800, 600, "Particle Engine"), scene(800, 600)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Application& Application::getInstance()
{
	static Application application;
	return application;
}

std::string fire = "Resources/Textures/fire.png";
std::string smoke = "Resources/Textures/smoke.png";
std::string container = "Resources/Textures/container.jpg";

void Application::run()
{
	Random::init();

	//texture type, wrapping method S, wrapping method T, wrapping method R, min filter, mag filter, internal format, format
	Texture texSmoke(smoke, 0, GL_TEXTURE_2D, GL_REPEAT, GL_REPEAT, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_RGBA, GL_RGBA, ',');
	Texture texFire(fire, 0, GL_TEXTURE_2D, GL_REPEAT, GL_REPEAT, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_RGBA, GL_RGBA, ',');
	Material matSmoke(&texSmoke, nullptr,
		Color4
		{
		glm::vec3{1.0f, 1.0f, 1.0f},
		glm::vec3{1.0f, 1.0f, 1.0f},
		glm::vec3{1.0f, 1.0f, 1.0f},
		1.0f,
		}, 1.0f);

	Material matFire(&texFire, nullptr, 
		Color4
		{
		glm::vec3{1.0f, 1.0f, 1.0f},
		glm::vec3{1.0f, 1.0f, 1.0f},
		glm::vec3{1.0f, 1.0f, 1.0f},
		1.0f,
		}, 1.0f);

	ParticleSystemProps propsSmoke
	{
		.startLifetime = 2.2f,
		.startSpeed = 1.8f,
		.startSize = 0.35f,
		.startColor = Color4{glm::vec4{1.0f, 1.0f, 1.0f, 0.5f}},
		.maxParticles = 850,
		.position = glm::vec3{0.0f, 1.1f, 0.0f},
	};

	ParticleSystemProps propsFire
	{
		.startLifetime = 0.8f,
		.startSpeed = 3.0f,
		.startSize = 0.75f,
		.startColor = Color4{glm::vec4{1.0f, 1.0f, 1.0f, 0.5f}},
		.maxParticles = 1500,
	};
	
	ParticleSystem psSmoke(propsSmoke, matSmoke, std::make_unique<ConeEmitter>(ConeEmitter{75.0f, 0.45f, glm::radians(32.0f)}));
	ParticleSystem psFire(propsFire, matFire, std::make_unique<ConeEmitter>(ConeEmitter{125.0f, 0.55f, glm::radians(25.0f)}));

	CubicBezierCurve<float> solBezierSmoke{0.72f, 0.38f, 0.24f, 0.01f};
	SizeOverLifetime* solSmoke = new SizeOverLifetime(solBezierSmoke);

	CubicBezierCurve<float> solBezierFire{0.99f, 0.79f, 0.53f, 0.05f};
	SizeOverLifetime* solFire = new SizeOverLifetime(solBezierFire);

	psSmoke.addComponent(solSmoke);
	psFire.addComponent(solFire);

	/*ColorBySpeed* cbs = new ColorBySpeed(0.5f, Color4{glm::vec4{1.0f, 0.0f, 0.0f, 1.0f}},
		0.9f, Color4{glm::vec4{1.0f, 0.93f, 0.0f, 1.0f}}, 1.25f, 2.0f);

	ps.addComponent(cbs);*/

	ColorOverLifetime* colSmoke = new ColorOverLifetime(0.0f, Color4{glm::vec4{0.5f, 0.5f, 0.5f, 0.7f}},
		1.0f, Color4{glm::vec4{1.0f, 1.0f, 1.0f, 0.25f}});

	ColorOverLifetime* colFire = new ColorOverLifetime(0.0f, Color4{glm::vec4{1.0f, 1.0f, 1.0f, 0.7f}},
		1.0f, Color4{glm::vec4{1.0f, 1.0f, 0.0f, 0.27f}});

	psSmoke.addComponent(colSmoke);
	psFire.addComponent(colFire);

	scene.createDirectionalLight(glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec3{1.0f, 1.0f, 1.0f});
	//scene.createPointLight(glm::vec3{3.0f, 3.0f, 3.0f}, glm::vec3{1.0f, 1.0f, 1.0f}, LightDistance::AD_100);

	uint32_t polygonModes[2] = {GL_FILL, GL_LINE};
	void (*glToggle[2])(GLenum) = {&glEnable, &glDisable};
	bool currentMode = 0;

	Time::start();


	while (!window.shouldClose())
	{
		glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
		glClear(clearMask);

		window.pollEvents();

		// Start the Dear ImGui frame
		// Editor::beginFrame();

		// Editor::ShowDemoWindow();

		if (Input::getKeyDown(KeyCode::KEY_X))
		{
			currentMode = !currentMode;
			glPolygonMode(GL_FRONT_AND_BACK, polygonModes[currentMode]);
			glToggle[currentMode](GL_CULL_FACE);
		}

		psFire.update();
		psSmoke.update();
		scene.update();
		//mr2.render();
		//mr.render();
		scene.render();
		

		// Editor::render();

		window.swapBuffers();
		window.endFrame();
		Time::endFrame();

		
	}

	glfwTerminate();
	// Editor::shutdown();
}