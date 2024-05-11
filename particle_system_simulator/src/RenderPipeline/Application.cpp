
#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <stdint.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

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

Application::Application() : window(1920, 1080, "Particle Engine"), scene(1920, 1080), editor()
{
	Gui::init(window.getNativeWindow());
	Random::init();

	Texture texSmoke("Resources/Textures/smoke.png",
		GL_TEXTURE_2D, GL_REPEAT, GL_REPEAT, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_RGBA, GL_RGBA, ',');

	Texture texFire("Resources/Textures/fire.png",
		GL_TEXTURE_2D, GL_REPEAT, GL_REPEAT, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_RGBA, GL_RGBA, ',');

	Texture texSnowflake("Resources/Textures/snowflake.png",
		GL_TEXTURE_2D, GL_REPEAT, GL_REPEAT, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_RGBA, GL_RGBA, ',');
	
	Texture texRaindrops("Resources/Textures/raindrops.png",
		GL_TEXTURE_2D, GL_REPEAT, GL_REPEAT, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_RGBA, GL_RGBA, ',');

	ResourceManager::addTexture(std::move(texSmoke), "smoke");
	ResourceManager::addTexture(std::move(texFire), "fire");
	ResourceManager::addTexture(std::move(texSnowflake), "snowflake");
	ResourceManager::addTexture(std::move(texRaindrops), "raindrops");
}

Application& Application::getInstance()
{
	static Application application;
	return application;
}

void Application::gameLoop(std::function<void()> frameLogic)
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

		frameLogic();
		editor.render();
		/*ImGui::ShowDemoWindow();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());*/

		window.swapBuffers();
		window.endFrame();
		Gui::endFrame();
		Time::endFrame();
	}

	Gui::shutdown();
	glfwTerminate();
}



void Application::run()
{
	Texture& texSmoke = *const_cast<Texture*>(ResourceManager::getTexture("smoke"));
	Texture& texFire = *const_cast<Texture*>(ResourceManager::getTexture("fire"));
	Texture& texSnowFlake = *const_cast<Texture*>(ResourceManager::getTexture("snowflake"));
	Texture& texRaindrops = *const_cast<Texture*>(ResourceManager::getTexture("raindrops"));

	//texture type, wrapping method S, wrapping method T, wrapping method R, min filter, mag filter, internal format, format

	Material matRaindrops(&texRaindrops, nullptr,
		Color4
		{
		glm::vec3{1.0f, 1.0f, 1.0f},
		glm::vec3{1.0f, 1.0f, 1.0f},
		glm::vec3{1.0f, 1.0f, 1.0f},
		1.0f,
		}, 1.0f);

	Material matSnowflake(&texSnowFlake, nullptr,
		Color4
		{
		glm::vec3{1.0f, 1.0f, 1.0f},
		glm::vec3{1.0f, 1.0f, 1.0f},
		glm::vec3{1.0f, 1.0f, 1.0f},
		1.0f,
		}, 1.0f);

	Material matSmoke1(&texSmoke, nullptr,
		Color4
		{
		glm::vec3{1.0f, 1.0f, 1.0f},
		glm::vec3{0.0f, 0.0f, 0.0f},
		glm::vec3{0.0f, 0.0f, 0.0f},
		1.0f,
		}, 1.0f);

	Material matSmoke2(&texSmoke, nullptr,
		Color4
		{
		glm::vec3{1.0f, 1.0f, 1.0f},
		glm::vec3{0.0f, 0.0f, 0.0f},
		glm::vec3{0.0f, 0.0f, 0.0f},
		1.0f,
		}, 1.0f);

	Material matSmoke3(&texSmoke, nullptr,
		Color4
		{
		glm::vec3{1.0f, 1.0f, 1.0f},
		glm::vec3{0.0f, 0.0f, 0.0f},
		glm::vec3{0.0f, 0.0f, 0.0f},
		1.0f,
		}, 1.0f);

	Material matSmoke4(&texSmoke, nullptr,
		Color4
		{
		glm::vec3{1.0f, 1.0f, 1.0f},
		glm::vec3{0.0f, 0.0f, 0.0f},
		glm::vec3{0.0f, 0.0f, 0.0f},
		1.0f,
		}, 1.0f);

	Material matFire(&texFire, nullptr, 
		Color4
		{
		glm::vec3{1.0f, 1.0f, 1.0f},
		glm::vec3{0.0f, 0.0f, 0.0f},
		glm::vec3{0.0f, 0.0f, 0.0f},
		1.0f,
		}, 1.0f);


	ParticleSystemProps propsRaindrops
	{
		.startLifetime = 3.2f,
		.startSpeed = 1.0f,
		.startSize = 5.0f,
		.startColor = matRaindrops.getColor(),
		.maxParticles = 100,
		.position = glm::vec3{0.0f, 1.1f, -2.0f},
	};

	ParticleSystemProps propsSnowflake
	{
		.startLifetime = 3.2f,
		.startSpeed = 1.0f,
		.startSize = 5.0f,
		.startColor = matSnowflake.getColor(),
		.maxParticles = 100,
		.position = glm::vec3{0.0f, 1.1f, -2.0f},
	};

	ParticleSystemProps propsSmoke1
	{
		.startLifetime = 3.2f,
		.startSpeed = 1.0f,
		.startSize = 5.0f,
		.startColor = matSmoke1.getColor(),
		.maxParticles = 100,
		.position = glm::vec3{0.0f, 1.1f, -2.0f},
	};

	ParticleSystemProps propsSmoke2
	{
		.startLifetime = 3.2f,
		.startSpeed = 1.0f,
		.startSize = 5.0f,
		.startColor = matSmoke2.getColor(),
		.maxParticles = 100,
		.position = glm::vec3{0.0f, 1.1f, -2.0f},
	};

	ParticleSystemProps propsSmoke3
	{
		.startLifetime = 3.2f,
		.startSpeed = 1.0f,
		.startSize = 5.0f,
		.startColor = matSmoke3.getColor(),
		.maxParticles = 100,
		.position = glm::vec3{0.0f, 1.1f, -2.0f},
	};

	ParticleSystemProps propsSmoke4
	{
		.startLifetime = 3.2f,
		.startSpeed = 1.0f,
		.startSize = 5.0f,
		.startColor = matSmoke4.getColor(),
		.maxParticles = 100,
		.position = glm::vec3{0.0f, 1.1f, -2.0f},
	};

	ParticleSystemProps propsFire
	{
		.startLifetime = 0.5f,
		.startSpeed = 3.0f,
		.startSize = 5.75f,
		.startColor = matFire.getColor(),
		.maxParticles = 1,
	};


	//ParticleSystem psRain("Rain", propsRaindrops, matRaindrops, std::make_unique<ConeEmitter>(ConeEmitter{20.0f, 0.35f, glm::radians(45.0f)}));
	//ParticleSystem psSnow("Snow", propsSnowflake, matSnowflake, std::make_unique<ConeEmitter>(ConeEmitter{20.0f, 0.35f, glm::radians(45.0f)}));
	ParticleSystem psSmoke1("Smoke", propsSmoke1, matSmoke1, std::make_unique<ConeEmitter>(ConeEmitter{20.0f, 0.35f, glm::radians(45.0f)}));
	ParticleSystem psSmoke2("Smoke", propsSmoke2, matSmoke2, std::make_unique<ConeEmitter>(ConeEmitter{20.0f, 0.35f, glm::radians(45.0f)}));
	ParticleSystem psSmoke3("Smoke", propsSmoke3, matSmoke3, std::make_unique<ConeEmitter>(ConeEmitter{20.0f, 0.35f, glm::radians(45.0f)}));
	ParticleSystem psSmoke4("Smoke", propsSmoke4, matSmoke4, std::make_unique<ConeEmitter>(ConeEmitter{20.0f, 0.35f, glm::radians(45.0f)}));
	ParticleSystem psFire("Fire", propsFire, matFire, std::make_unique<ConeEmitter>(ConeEmitter{45.0f, 0.75f, glm::radians(15.0f)}));

	CubicBezierCurve<float> solBezierRain{0.00001f, 0.000005f, 0.000001f, 0.0000001f};
	SizeOverLifetime* solRain = new SizeOverLifetime(solBezierRain);

	CubicBezierCurve<float> solBezierSnow{0.00001f, 0.000005f, 0.000001f, 0.0000001f};
	SizeOverLifetime* solSnow = new SizeOverLifetime(solBezierSnow);

	CubicBezierCurve<float> solBezierSmoke1{0.00001f, 0.000005f, 0.000001f, 0.0000001f};
	SizeOverLifetime* solSmoke1 = new SizeOverLifetime(solBezierSmoke1);

	CubicBezierCurve<float> solBezierSmoke2{0.00001f, 0.000005f, 0.000001f, 0.0000001f};
	SizeOverLifetime* solSmoke2 = new SizeOverLifetime(solBezierSmoke2);

	CubicBezierCurve<float> solBezierSmoke3{0.00001f, 0.000005f, 0.000001f, 0.0000001f};
	SizeOverLifetime* solSmoke3 = new SizeOverLifetime(solBezierSmoke3);

	CubicBezierCurve<float> solBezierSmoke4{0.00001f, 0.000005f, 0.000001f, 0.0000001f};
	SizeOverLifetime* solSmoke4 = new SizeOverLifetime(solBezierSmoke4);

	CubicBezierCurve<float> solBezierFire{0.99f, 0.79f, 0.53f, 0.05f};
	SizeOverLifetime* solFire = new SizeOverLifetime(solBezierFire);

	RotationBySpeed* rbsSmoke = new RotationBySpeed(0.0f, 1.0f, CubicBezierCurve<float>(1.0f, 2.0f, 3.0f, 4.0f));

	psSmoke1.addComponent(solSmoke1);
	psSmoke2.addComponent(solSmoke2);
	psSmoke3.addComponent(solSmoke3);
	psSmoke4.addComponent(solSmoke4);
	//psSnow.addComponent(solSnow);
	//psRain.addComponent(solRain);
	psFire.addComponent(solFire);
	

	ColorOverLifetime* colSmoke1 = new ColorOverLifetime(0.0f, Color4{glm::vec4{0.5f, 0.5f, 0.5f, 0.7f}},
		1.0f, Color4{glm::vec4{1.0f, 1.0f, 1.0f, 0.25f}});

	ColorOverLifetime* colSmoke2 = new ColorOverLifetime(0.0f, Color4{glm::vec4{0.5f, 0.5f, 0.5f, 0.7f}},
		1.0f, Color4{glm::vec4{1.0f, 1.0f, 1.0f, 0.25f}});

	ColorOverLifetime* colSmoke3 = new ColorOverLifetime(0.0f, Color4{glm::vec4{0.5f, 0.5f, 0.5f, 0.7f}},
		1.0f, Color4{glm::vec4{1.0f, 1.0f, 1.0f, 0.25f}});

	ColorOverLifetime* colSmoke4 = new ColorOverLifetime(0.0f, Color4{glm::vec4{0.5f, 0.5f, 0.5f, 0.7f}},
		1.0f, Color4{glm::vec4{1.0f, 1.0f, 1.0f, 0.25f}});

	ColorOverLifetime* colFire = new ColorOverLifetime(0.0f, Color4{glm::vec4{1.0f, 1.0f, 1.0f, 0.7f}},
		1.0f, Color4{glm::vec4{1.0f, 1.0f, 0.0f, 0.27f}});

	psSmoke1.addComponent(colSmoke1);
	psSmoke2.addComponent(colSmoke2);
	psSmoke3.addComponent(colSmoke3);
	psSmoke4.addComponent(colSmoke4);
	psFire.addComponent(colFire);

	CubicBezierCurve<glm::vec3> solBezierRain2{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{5.0f, 2.0f, 0.0f},
		glm::vec3{-5.3f, -2.8f, 0.0f}, glm::vec3{-2.1f, -0.2f, 0.0f}};

	ForceOverLifetime* folRain = new ForceOverLifetime(solBezierRain2);

	//psRain.addComponent(folRain);

	CubicBezierCurve<glm::vec3> solBezierSnow2{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{5.0f, 2.0f, 0.0f},
		glm::vec3{-5.3f, -2.8f, 0.0f}, glm::vec3{-2.1f, -0.2f, 0.0f}};

	ForceOverLifetime* folSnow = new ForceOverLifetime(solBezierSnow2);

	//psSnow.addComponent(folSnow);

	CubicBezierCurve<glm::vec3> solBezierSmoke2_1{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{5.0f, 2.0f, 0.0f},
			glm::vec3{-5.3f, -2.8f, 0.0f}, glm::vec3{-2.1f, -0.2f, 0.0f}};

	ForceOverLifetime* folSmoke1 = new ForceOverLifetime(solBezierSmoke2_1);

	CubicBezierCurve<glm::vec3> solBezierSmoke2_2{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{5.0f, 2.0f, 0.0f},
		glm::vec3{-5.3f, -2.8f, 0.0f}, glm::vec3{-2.1f, -0.2f, 0.0f}};

	ForceOverLifetime* folSmoke2 = new ForceOverLifetime(solBezierSmoke2_2);

	CubicBezierCurve<glm::vec3> solBezierSmoke2_3{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{5.0f, 2.0f, 0.0f},
		glm::vec3{-5.3f, -2.8f, 0.0f}, glm::vec3{-2.1f, -0.2f, 0.0f}};

	ForceOverLifetime* folSmoke3 = new ForceOverLifetime(solBezierSmoke2_3);

	CubicBezierCurve<glm::vec3> solBezierSmoke2_4{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{5.0f, 2.0f, 0.0f},
		glm::vec3{-5.3f, -2.8f, 0.0f}, glm::vec3{-2.1f, -0.2f, 0.0f}};

	ForceOverLifetime* folSmoke4 = new ForceOverLifetime(solBezierSmoke2_4);
	
	psSmoke1.addComponent(folSmoke1);
	psSmoke2.addComponent(folSmoke2);
	psSmoke3.addComponent(folSmoke3);
	psSmoke4.addComponent(folSmoke4);

	scene.createDirectionalLight(glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec3{1.0f, 1.0f, 1.0f});

	editor.addParticleSystem(psSmoke1);
	//editor.addParticleSystem(psSmoke2);
	//editor.addParticleSystem(psSmoke3);
	//editor.addParticleSystem(psSmoke4);
	//editor.addParticleSystem(psSnow);
	//editor.addParticleSystem(psRain);
	editor.addParticleSystem(psFire);

	gameLoop([&]()
		{
			scene.update();
			psFire.update();
			//psRain.update();
			//psSnow.update();
			psSmoke1.update();
			//psSmoke2.update();
			//psSmoke3.update();
			//psSmoke4.update();
			scene.render();
		});
}