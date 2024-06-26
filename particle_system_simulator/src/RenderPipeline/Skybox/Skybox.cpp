#include <glm/ext/quaternion_float.hpp>
#include "GeneralUtility/stb_wrapper/stb_wrapper.h"
#include "MeshConstruction/Shapes.h"
#include "RenderPipeline/Shader/Shader.h"
#include "Skybox.h"

Skybox::Skybox(std::string_view cubemapTexture) : shader(Shader::skyboxShader()),
skybox(createCube()), texture(cubemapTexture, GL_TEXTURE_CUBE_MAP)
{
	skybox.initializeOrAddInstance(shader.getVertexAttributes());
}

void Skybox::render(const glm::mat4& view, const glm::mat4& projection)
{
	glCullFace(GL_FRONT);
	glDisable(GL_BLEND);

	shader.useShader();

	shader.setMatrix4("view_projection", projection * glm::mat4(glm::mat3(view)));
	texture.useTexture(0);
	skybox.draw();

	glEnable(GL_BLEND);
	glCullFace(GL_BACK);
}