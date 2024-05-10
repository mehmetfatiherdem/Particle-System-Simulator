#pragma once

#include <string_view>
#include <stdint.h>
#include "GeneralUtility/gl2fw3.h"

class Texture
{
private:
	GLuint textureID;
	GLenum textureType;
	uint32_t width;
	uint32_t height;

public:
	Texture() = delete;
	Texture(std::string_view textureAddress, GLenum textureType = GL_TEXTURE_2D,
		GLint textureWrappingMethod_S = GL_REPEAT, GLint textureWrappingMethod_T = GL_REPEAT,
		GLint textureWrappingMethod_R = GL_CLAMP_TO_EDGE, GLint textureMinFilter = GL_LINEAR,
		GLint textureMagFilter = GL_LINEAR, GLint internalFormat = GL_RGBA, GLenum format = GL_RGBA, char addressDelimiter = ',');

	Texture(unsigned char* textureData, uint32_t width, uint32_t height, GLenum textureType = GL_TEXTURE_2D,
		GLint textureWrappingMethod_S = GL_REPEAT, GLint textureWrappingMethod_T = GL_REPEAT,
		GLint textureWrappingMethod_R = GL_CLAMP_TO_EDGE, GLint textureMinFilter = GL_LINEAR,
		GLint textureMagFilter = GL_LINEAR, GLint internalFormat = GL_RGBA, GLenum format = GL_RGBA);

	Texture(const Texture& texture) = delete;
	Texture(Texture&& texture) noexcept;
	~Texture();

	Texture& operator=(const Texture& texture) = delete;
	Texture& operator=(Texture&& texture) noexcept;

	void useTexture(uint32_t textureUnit) const;

	GLuint getTextureID() const { return textureID; }
};