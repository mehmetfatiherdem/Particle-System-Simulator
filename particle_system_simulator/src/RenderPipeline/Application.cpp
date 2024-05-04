#include <iostream>
#include <cstring>
#include <cmath>

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
		.startLifetime = 3.2f,
		.startSpeed = 1.0f,
		.startSize = 5.0f,
		.startColor = Color4{glm::vec4{1.0f, 1.0f, 1.0f, 0.5f}},
		.maxParticles = 2000,
		.position = glm::vec3{0.0f, 1.1f, -2.0f},
	};

	ParticleSystemProps propsFire
	{
		.startLifetime = 0.5f,
		.startSpeed = 3.0f,
		.startSize = 5.75f,
		.startColor = Color4{glm::vec4{1.0f, 1.0f, 1.0f, 0.5f}},
		.maxParticles = 550,
	};
	//low poly
	//ParticleSystem psSmoke(propsSmoke, matSmoke, std::make_unique<ConeEmitter>(ConeEmitter{200.0f, 0.25f, glm::radians(25.0f)}));
	//3d realistic
	ParticleSystem psSmoke(propsSmoke, matSmoke, std::make_unique<ConeEmitter>(ConeEmitter{20.0f, 0.35f, glm::radians(45.0f)}));
	ParticleSystem psFire(propsFire, matFire, std::make_unique<ConeEmitter>(ConeEmitter{45.0f, 0.75f, glm::radians(15.0f)}));

	CubicBezierCurve<float> solBezierSmoke{0.00001f, 0.000005f, 0.000001f, 0.0000001f};
	SizeOverLifetime* solSmoke = new SizeOverLifetime(solBezierSmoke);

	CubicBezierCurve<float> solBezierFire{0.99f, 0.79f, 0.53f, 0.05f};
	SizeOverLifetime* solFire = new SizeOverLifetime(solBezierFire);

	//to make it realistic comment this line
	//psSmoke.addComponent(solSmoke);
	//psFire.addComponent(solFire);

	/*ColorBySpeed* cbs = new ColorBySpeed(0.5f, Color4{glm::vec4{1.0f, 0.0f, 0.0f, 1.0f}},
		0.9f, Color4{glm::vec4{1.0f, 0.93f, 0.0f, 1.0f}}, 1.25f, 2.0f);

	ps.addComponent(cbs);*/

	ColorOverLifetime* colSmoke = new ColorOverLifetime(0.0f, Color4{glm::vec4{0.5f, 0.5f, 0.5f, 0.7f}},
		1.0f, Color4{glm::vec4{1.0f, 1.0f, 1.0f, 0.25f}});

	ColorOverLifetime* colFire = new ColorOverLifetime(0.0f, Color4{glm::vec4{1.0f, 1.0f, 1.0f, 0.7f}},
		1.0f, Color4{glm::vec4{1.0f, 1.0f, 0.0f, 0.27f}});

	psSmoke.addComponent(colSmoke);
	psFire.addComponent(colFire);
	CubicBezierCurve<glm::vec3> solBezierSmoke2{glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{5.0f, 2.0f, 0.0f},
			glm::vec3{-5.3f, -2.8f, 0.0f}, glm::vec3{-2.1f, -0.2f, 0.0f}};
	ForceOverLifetime* volSmoke = new ForceOverLifetime(solBezierSmoke2);
	
	psSmoke.addComponent(volSmoke);

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

		if (Input::getKeyDown(KeyCode::KEY_X))
		{
			currentMode = !currentMode;
			glPolygonMode(GL_FRONT_AND_BACK, polygonModes[currentMode]);
			glToggle[currentMode](GL_CULL_FACE);
		}

		//psFire.update();
		psSmoke.update();
		scene.update();
		//mr2.render();
		//mr.render();
		scene.render();

		window.swapBuffers();
		window.endFrame();
		Time::endFrame();
	}

	glfwTerminate();
}