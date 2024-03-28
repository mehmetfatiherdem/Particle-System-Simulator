#pragma once

#include <string_view>
#include <GL/glew.h>
#include "RenderPipeline/Shader/Shader.h"

class Camera;

class Skybox
{
private:
	GLuint textureID;
	GLuint VAO;
	GLuint VBO;
	GLuint IBO;
	unsigned int indicesSize;
	Shader shader;

public:
	Skybox(std::string_view right, std::string_view left, std::string_view back, std::string_view front, std::string_view top, std::string_view bottom);
	~Skybox();

	void render();
};