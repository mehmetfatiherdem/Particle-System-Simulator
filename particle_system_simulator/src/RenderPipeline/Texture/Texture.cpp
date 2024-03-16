#include <cassert>
#include <iostream>
#include <stb/stb_image.h>
#include "Texture.h"

Texture::Texture(std::string_view textureAddress, unsigned int textureUnit, GLenum textureType, GLint textureWrappingMethod_S,
	GLint textureWrappingMethod_T, GLint textureMinFilter, GLint textureMagFilter,GLint internalFormat,GLenum format)
	: textureUnit(textureUnit), textureType(textureType), textureID(0)
{
	//assert(textureUnit < 16 && ("Failed to set the texture unit to " + textureUnit + ", maximum allowed texture unit is 15!"));

	stbi_set_flip_vertically_on_load(true);
	unsigned char* textureData = stbi_load(textureAddress.data(), &width, &height, &bitDepth, 0);

	if(!textureData)
	{
		std::cerr << "Error loading the texture at file location : " << textureAddress << '\n';
		return;
	}

	glGenTextures(1, &textureID);
	glBindTexture(textureType, textureID);


	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, textureWrappingMethod_S);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, textureWrappingMethod_T);
	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, textureMinFilter);
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, textureMagFilter);

	glTexImage2D(textureType, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(textureType);
	glBindTexture(textureType, 0);

	stbi_image_free(textureData);
}

Texture::Texture(Texture&& texture) noexcept : textureID(texture.textureID), textureType(texture.textureType), textureUnit(texture.textureUnit),
	width(texture.width), height(texture.height), bitDepth(texture.bitDepth)
{
	texture.textureID = 0;
	texture.width = 0;
	texture.height = 0;
	texture.bitDepth = 0;
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
}

Texture& Texture::operator=(Texture&& texture) noexcept
{
	this->textureID = texture.textureID;
	this->textureType = texture.textureType;
	this->width = texture.width;
	this->height = texture.height;
	this->bitDepth = texture.bitDepth;
	texture.textureID = 0;
	texture.width = 0;
	texture.height = 0;
	texture.bitDepth = 0;
}

void Texture::useTexture() const
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(textureType, textureID);
}

void Texture::setTextureUnit(unsigned int textureUnit)
{
	//assert(textureUnit < 16 && ("Failed to set the texture unit to " + textureUnit + ", maximum allowed texture unit is 15!"));
	this->textureUnit = textureUnit;
}