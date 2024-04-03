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
#include "RenderPipeline/Application.h"

Application::Application() : window(800, 600, "Particle Engine"), scene(this->window.getAspectRatio()) 
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
}