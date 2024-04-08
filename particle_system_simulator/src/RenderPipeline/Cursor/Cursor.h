#pragma once

#include <string_view>
#include "RenderPipeline/Shader/Shader.h"
#include "RenderPipeline/Texture/Texture.h"

class Mesh;

class Cursor
{
private:
	Mesh cursor;
	Texture texture;
	Shader& shader;
	float scale;

public:
	Cursor(std::string_view texture, float scale);

	void render();
};