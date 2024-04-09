#pragma once

#include <string_view>
#include "GeneralUtility/gl2fw3.h"
#include "RenderPipeline/Texture/Texture.h"
#include "RenderPipeline/Mesh/Mesh.h"

class Shader;

class Skybox
{
private:
	Mesh skybox;
	Texture texture;
	Shader& shader;

public:
	Skybox(std::string_view cubemapTexture);

	void render(const glm::mat4& view, const glm::mat4& projection);
};