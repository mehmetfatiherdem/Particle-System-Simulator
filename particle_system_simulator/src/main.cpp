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
#include "RenderPipeline/Object/MeshRenderer.h"
#include "RenderPipeline/Shader/ShaderManagement/GlobalShaderManager.h"
#include "RenderPipeline/Scene/Scene.h"
#include "MeshConstruction/Shapes.h"
#include "RenderPipeline/Mesh/Data/MeshProperties.h"
#include "RenderPipeline/Application.h"

int main()
{
    initializeSettings();
    Application& app = Application::getInstance();
    Scene& scene = app.getScene();
    Window& window = app.getWindow();

    Material* material = new Material{nullptr, nullptr, Color4{glm::vec4{0.6f, 0.65f, 0.7f, 1.0f}}, 1.0f, 1.0f};
    Mesh* mesh = createCube(MeshProperties{false});

    auto x = scene.createObject(glm::vec3{0,0,0}, mesh, material);

    Camera& cam = scene.getCamera();
    Transform& camt = cam.getTransform();
    cam.setCameraType(CameraType::Perspective);

    scene.createDirectionalLight(glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec3{0.9f, 0.6f, 0.6f});

    while(!window.shouldClose())
    {
        glClearColor(0.6, 0.5, 0.4, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene.render();

        window.swapBuffers();
        window.pollEvents();
    }
}