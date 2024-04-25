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

Application::Application() : window(800, 600, "Particle Engine"), scene(800, 600) 
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
}

Application& Application::getInstance()
{
    static Application application;
    return application;
}

void Application::run()
{
    Mesh mesh = createCube();
    auto obj = scene.createObject(TransformProps{glm::vec3{1.0,2.0,3.0}, glm::vec3{1.0, 0.75, 0.63}}, mesh, Shader::genericShader(), Material::defaultMaterial());
    obj = scene.createObject(obj);
    obj->getTransform().setPosition(glm::vec3{1.0f, 1.0f, 1.0f});

    scene.createDirectionalLight(glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec3{0.9f, 0.6f, 0.6f});

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

        scene.update();
        scene.render();

        window.swapBuffers();
        window.endFrame();
        Time::endFrame();
    }
}