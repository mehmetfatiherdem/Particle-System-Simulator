#include <iostream>
#include <vector>
#include "GeneralUtility/stb_wrapper/stb_wrapper.h"
#include "Skybox.h"
#include "MeshConstruction/Shapes.h"
#include "RenderPipeline/Mesh/Mesh.h"

Skybox::Skybox(std::string_view cubemapTexture) : shader(Shader::skyboxShader()),
	skybox(createCube()), texture(cubemapTexture, 0, GL_TEXTURE_CUBE_MAP) { }

void Skybox::render(const glm::mat4& view, const glm::mat4& projection)
{
	glDepthMask(GL_FALSE);
	glCullFace(GL_FRONT);

	shader.useShader();
	shader.setMatrix4("view_projection", projection * glm::mat4(glm::mat3(view)));
	texture.useTexture();
	skybox.draw();

	glCullFace(GL_BACK);
	glDepthMask(GL_TRUE);
}