#include "MeshConstruction/Shapes.h"
#include "Cursor.h"

Cursor::Cursor(std::string_view texture, float scale) : cursor(createQuad()), texture(texture, 0),
	shader(Shader::cursorShader()), scale(scale) { }

void Cursor::render()
{
	glDepthMask(GL_FALSE);

	shader.useShader();
	texture.useTexture();
	cursor.draw();

	glDepthMask(GL_TRUE);
}