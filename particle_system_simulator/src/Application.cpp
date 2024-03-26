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
#include "RenderPipeline/Shader/Shader.h"
#include "RenderPipeline/Texture/Texture.h"
#include "RenderPipeline/Material/Material.h"
#include "RenderPipeline/Camera/Camera.h"
#include "RenderPipeline/Object/RenderableObject.h"
#include "RenderPipeline/Shader/ShaderManagement/GlobalShaderManager.h"
#include "RenderPipeline/Scene/Scene.h"
#include "MeshConstruction/Shapes.h"
#include "RenderPipeline/Mesh/Data/MeshProperties.h"

int main()
{
    Initialization::initializeAll();    //Somehow doesn't initialize opengl settings so I initialize them again before the render loop

    Window window{800,600, "test run", true};

    Shader shader{"Resources/Shaders/instanced.vert", "Resources/Shaders/generic.frag"};
    //Material material{&shader, nullptr, nullptr, Color4{glm::vec4{139.0f/ 255.0f, 69.0f / 255.0f, 19.0f / 255.0f, 1.0f}}, 1, 1};
    Material material(&shader, nullptr, nullptr, Color4{glm::vec4{0.0f, 1.0f, 1.0f, 1.0f}}, 1.0f, 1.0f);
    Mesh mesh = createSphere(MeshProperties{true}, 1000, true);

    RenderableObject obj1{glm::vec3{2.0f, 0.0f, 0.0f}, &mesh, &material};
    RenderableObject obj2{&mesh, &material};
    RenderableObject obj3{glm::vec3{-2.0f, 0.0f, 0.0f}, &mesh, &material};

    Scene scene{window.getAspectRatio()};

    Camera& cam = scene.getCamera();
    cam.setCameraType(CameraType::Perspective);
    Transform& camTransform = cam.getTransform();
    camTransform.setPosition(glm::vec3{0.0f, 0.0f, 8.0f});

    //scene.createSpotLight(glm::vec3{0.0f, 30.0f, 0.0f}, glm::vec3{0.0f, -1.0f, 0.0f}, glm::vec3{1.0f, 1.0f, 1.0f}, LightDistance::AD_100, glm::radians(30.0f), glm::radians(45.0f));

    //scene.createSpotLight(glm::vec3{0.0f, 0.0f, 20.0f}, glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec3{1.0f, 1.0f, 1.0f}, LightDistance::AD_100, glm::radians(50.0f), glm::radians(70.0f));

    //SpotLight* slight = scene.createSpotLight(glm::vec3{25.0f, 0.0f, 10.0f}, glm::vec3{-1.0f, 0.0f, 0.0f}, glm::vec3{1.0f, 1.0f, 1.0f}, LightDistance::AD_100, glm::radians(50.0f), glm::radians(70.0f));

    //slight->lookAt(glm::vec3{0.0f, 0.0f, 0.0f});

    //slight = scene.createSpotLight(glm::vec3{38.0f, 0.0f, 20.0f}, glm::vec3{1.0f, 0.0f, 0.0f}, glm::vec3{1.0f, 1.0f, 1.0f}, LightDistance::AD_100, glm::radians(45.0f), glm::radians(60.0f));

    //slight->lookAt(glm::vec3{0.0f, 0.0f, 0.0f});

    //scene.createPointLight(glm::vec3{0.0f,0.0f,10.0f}, Color3{glm::vec3{1.0f, 1.0f, 1.0f}}, LightDistance::AD_100);
    //scene.createPointLight(glm::vec3{10.0f,5.0f,25.0f}, Color3{glm::vec3{1.0f, 1.0f, 1.0f}}, LightDistance::AD_100);
    //scene.createPointLight(glm::vec3{-7.0f,30.0f,5.0f}, Color3{glm::vec3{1.0f, 1.0f, 1.0f}}, LightDistance::AD_100);
    //scene.createPointLight(glm::vec3{3.0f,5.0f,50.0f}, Color3{glm::vec3{1.0f, 1.0f, 1.0f}}, LightDistance::AD_100);
    //scene.createPointLight(glm::vec3{3.0f,5.0f,4.0f}, Color3{glm::vec3{1.0f, 1.0f, 1.0f}}, LightDistance::AD_100);

    scene.createDirectionalLight(glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec3{0.1f, 0.1f, 0.1f});
    scene.createDirectionalLight(glm::normalize(glm::vec3{6.0f, -8.0f, -10.0f}), glm::vec3{0.1f, 0.1f, 0.1f});

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

        scene.update();
        obj1.render();
        obj2.render();
        obj3.render();

        window.swapBuffers();
        window.pollEvents();
    }
}

