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
	glEnable(GL_CULL_FACE);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
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

    Texture texture(container, 0, GL_TEXTURE_2D, GL_REPEAT, GL_REPEAT, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, GL_RGB, GL_RGB, ',');

    /*Mesh mesh = createCube();
    Material material(nullptr, nullptr,
        Color4{glm::vec4{0.0f, 0.0f, 0.0f, 0.0f},
        glm::vec4{1.0f, 1.0f, 1.0f, 1.0f},
        glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}}, 1.0f);*/

    /*auto obj = scene.createObject(TransformProps{glm::vec3{1.0,2.0,3.0}}, mesh, Shader::genericShader(), material);
    obj = scene.createObject(obj);
    obj->getTransform().setPosition(glm::vec3{1.0f, 1.0f, 1.0f});*/

    ParticleSystemProps psProps
    {
        .startLifetime = 1.25f,
        .startSpeed = 1.25f,
        .startSize = 0.5f,
		.startColor = Color4{glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}},
        .maxParticles = 250,
    };

    ParticleSystem ps(psProps, std::move(texture), std::make_unique<ConeEmitter>(ConeEmitter{30.0f, 0.25f, 0.0f}));

	CubicBezierCurve<float> solCurve{1.0f, 0.50f, 0.20f, 0.0f};
    SizeOverLifetime* sol = new SizeOverLifetime(solCurve);
	ps.addComponent(sol);

    /*ColorBySpeed* cbs = new ColorBySpeed(0.5f, Color4{glm::vec4{1.0f, 0.0f, 0.0f, 1.0f}},
        0.9f, Color4{glm::vec4{1.0f, 0.93f, 0.0f, 1.0f}}, 1.25f, 2.0f);

	ps.addComponent(cbs);*/

    ColorOverLifetime* col = new ColorOverLifetime(0.0f, Color4{glm::vec4{1.0f, 0.0f, 0.0f, 1.0f}},
        0.9f, Color4{glm::vec4{1.0f, 1.0f, 0.0f, 1.0f}});

	ps.addComponent(col);

    //scene.createDirectionalLight(glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec3{1.0f, 1.0f, 1.0f});
	scene.createPointLight(glm::vec3{3.0f, 3.0f, 3.0f}, glm::vec3{1.0f, 1.0f, 1.0f}, LightDistance::AD_100);

    uint32_t polygonModes[2] = {GL_FILL, GL_LINE};
    void (*glToggle[2])(GLenum) = {&glEnable, &glDisable};
    bool currentMode = 0;

    Time::start();

    while(!window.shouldClose())
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(clearMask);

        window.pollEvents();

        if(Input::getKeyDown(KeyCode::KEY_X))
        {
            currentMode = !currentMode;
            glPolygonMode(GL_FRONT_AND_BACK, polygonModes[currentMode]);
            glToggle[currentMode](GL_CULL_FACE);
        }

		ps.update();
        scene.update();
        scene.render();

        window.swapBuffers();
        window.endFrame();
        Time::endFrame();
    }

	glfwTerminate();
}