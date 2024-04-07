#include "MeshConstruction/Shapes.h"
#include "RenderPipeline/Mesh/Data/MeshProperties.h"
#include "Cursor.h"

Cursor::Cursor(std::string_view texture, float scale) : cursor(createQuad(VertexAttributes::cursor())), texture(texture, 0),
	shader("Resources/Shaders/Cursor/cursor.vert", "Resources/Shaders/Cursor/cursor.frag"), scale(scale) { }

Cursor::~Cursor()
{
	delete cursor;
}

void Cursor::render()
{
	glDepthMask(GL_FALSE);

	shader.useShader();
	texture.useTexture();
	cursor->draw();

	glDepthMask(GL_TRUE);
}