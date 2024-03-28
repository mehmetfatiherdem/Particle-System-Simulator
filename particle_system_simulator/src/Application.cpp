#include <iostream>
#include <cstring>
#include <cmath>

#include <GL/glew.h>
#include <variant>
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
#include "RenderPipeline/Object/MeshRenderer.h"
#include "RenderPipeline/Shader/ShaderManagement/GlobalShaderManager.h"
#include "RenderPipeline/Scene/Scene.h"
#include "MeshConstruction/Shapes.h"
#include "RenderPipeline/Mesh/Data/MeshProperties.h"

void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    std::cerr << "OpenGL Debug Message: " << message << std::endl;
}

int main()
{
    Initialization::initializeAll();    //Somehow doesn't initialize opengl settings so I initialize them again before the render loop
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
    glDebugMessageCallbackARB(debugCallback, nullptr);  //TODO: learn how to actually make this work

    Window window{800,600, "test run", true};

    Material* material = new Material{nullptr, nullptr, Color4{glm::vec4{0.0f, 1.0f, 1.0f, 1.0f}}, 1.0f, 1.0f};
    Mesh* mesh = createSphere(MeshProperties{true}, 1000, true);
    Scene scene{window.getAspectRatio()};

    scene.createObject(mesh, material);
    scene.createObject(glm::vec3{1.0f,1.0f,-1.0f}, mesh, material);
    scene.createObject(glm::vec3{2.0f, 2.0f, -2.0f}, mesh, material);

    Camera& cam = scene.getCamera();
    cam.setCameraType(CameraType::Perspective);
    cam.getTransform().setPosition(glm::vec3{0.0f,0.0f, 5.0f});

    scene.createPointLight(glm::vec3{0,0,0}, glm::vec3{1,1,1}, LightDistance::AD_100);
    scene.createDirectionalLight(glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec3{0.1f, 0.1f, 0.1f});
    scene.createDirectionalLight(glm::normalize(glm::vec3{6.0f, -8.0f, -10.0f}), glm::vec3{0.1f, 0.1f, 0.1f});

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);

    while(!window.shouldClose())
    {
        glClearColor(0.6, 0.5, 0.4, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene.render();

        window.swapBuffers();
        window.pollEvents();
    }
}
