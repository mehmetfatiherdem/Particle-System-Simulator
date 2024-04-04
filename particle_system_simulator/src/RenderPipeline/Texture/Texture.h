#pragma once

#include <string_view>
#include "GeneralUtility/gl2fw3.h"

class Texture
{
private:
	GLuint textureID;
	GLenum textureType;
	int width, height, bitDepth;
	unsigned int textureUnit;

public:
	Texture() = delete;
	Texture(std::string_view textureAddress, unsigned int textureUnit, GLenum textureType = GL_TEXTURE_2D, GLint textureWrappingMethod_S = GL_REPEAT,
		GLint textureWrappingMethod_T = GL_REPEAT, GLint textureMinFilter = GL_LINEAR, GLint textureMagFilter = GL_LINEAR,
		GLint internalFormat = GL_RGBA, GLenum format = GL_RGBA);
	Texture(Texture&& texture) noexcept;
	~Texture();

	Texture& operator=(Texture&& texture) noexcept;

	void useTexture() const;

	unsigned int getTextureUnit() const { return textureUnit; }
	void setTextureUnit(unsigned int textureUnit);
};