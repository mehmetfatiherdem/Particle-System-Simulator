#include <iostream>
#include <cstring>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>

#include "RenderPipeline/Initialization/Initialization.h"
#include "UserInterface/Window.h"
#include "RenderPipeline/Light/PointLight.h"
#include "RenderPipeline/Light/DirectionalLight.h"
#include "RenderPipeline/Light/SpotLight.h"
#include "RenderPipeline/Transform/Transform.h"
#include "RenderPipeline/Mesh/Mesh.h"
#include "RenderPipeline/Object/MeshRenderer.h"
#include "RenderPipeline/Shader/Shader.h"
#include "RenderPipeline/Texture/Texture.h"
#include "RenderPipeline/Material/Material.h"
#include "RenderPipeline/Camera/Camera.h"
#include "RenderPipeline/Object/RenderableObject.h"
#include "RenderPipeline/Shader/ShaderManagement/GlobalShaderManager.h"
#include "RenderPipeline/Scene/Scene.h"
#include "MeshConstruction/Shapes.h"

int main()
{
    Initialization::initializeAll();    //Somehow doesn't initialize opengl settings so I initialize them again before the render loop

    Window window{800,600, "test run", true};

    //Mesh mesh = createQuad();
    Mesh mesh = createSphere(1000, true);

    Shader shader{"Resources/Shaders/vertex.vert", "Resources/Shaders/fragment.frag"};
    Material material{&shader, nullptr, nullptr, Color4{glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}}, 1, 1};
    MeshRenderer meshRenderer{&mesh, &material};
    Transform transform{};
    RenderableObject object{transform, meshRenderer};

    Scene scene{window.getAspectRatio()};

    Camera& cam = scene.getCamera();
    cam.setCameraType(CameraType::Perspective);
    Transform& camTransform = cam.getTransform();
    camTransform.setPosition(glm::vec3{0.0f, 0.0f, 5.0f});

    glm::vec3 rot{glm::radians(0.0f), 2*glm::radians(0.1f), 3*glm::radians(0.0f)};
    glm::vec3 deltaColor{0.0f, 0.001f, 0};
    unsigned int counter = 0;

    PointLight* light = scene.createPointLight(glm::vec3{0.0f,0.0f,2.0f}, Color3{glm::vec3{1.0f, 0.0f, 0.0f}}, LightDistance::AD_100);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);

    while(!window.shouldClose())
    {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto pos = camTransform.getPosition();

        if(window.getKeyAtIndex(GLFW_KEY_W))
            camTransform.setPosition(pos + glm::vec3{0, 0.1f,0});
        else if(window.getKeyAtIndex(GLFW_KEY_S))
            camTransform.setPosition(pos + glm::vec3{0, -0.1f, 0});
        if(window.getKeyAtIndex(GLFW_KEY_A))
            camTransform.setPosition(pos + glm::vec3{-0.1f, 0, 0});
        else if(window.getKeyAtIndex(GLFW_KEY_D))
            camTransform.setPosition(pos + glm::vec3{0.1f,0,0});

        object.getTransform().rotate(rot);

        light->setColor(light->getColor().ambient + deltaColor);
        scene.update();
        object.render();

        if(++counter % 1000 == 0)
        {
            counter = 0;
            deltaColor *= -1;
        }

        window.swapBuffers();
        window.pollEvents();
    }
}

