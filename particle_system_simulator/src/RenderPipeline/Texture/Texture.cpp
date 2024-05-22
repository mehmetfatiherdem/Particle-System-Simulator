#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "GeneralUtility/stb_wrapper/stb_wrapper.h"
#include "Texture.h"

void addCubemapSurface(GLenum targetSide, std::string_view file)
{
	int width, height, noOfChannels;
	unsigned char* data = w_stbi_load(file.data(), &width, &height, &noOfChannels, 0);

	if (data)
	{
		glTexImage2D(targetSide, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		w_stbi_image_free(data);
	}
	else
	{
		std::cerr << "Cubemap texture failed to load at path: " << file << "\n";
	}
}

Texture::Texture(std::string_view textureAddress, GLenum textureType, GLint textureWrappingMethod_S,
	GLint textureWrappingMethod_T, GLint textureWrappingMethod_R, GLint textureMinFilter, GLint textureMagFilter, GLint internalFormat,
	GLenum format, char addressDelimiter) : textureType(textureType), textureID(0)
{
	glGenTextures(1, &textureID);
	glBindTexture(textureType, textureID);

	if (textureType == GL_TEXTURE_CUBE_MAP)
	{
		std::istringstream iss{std::string(textureAddress)};

		uint32_t sides[6] = {
			GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		};

		std::string texture;

		for (size_t i = 0; i < 6; ++i)
		{
			std::getline(iss, texture, addressDelimiter);
			addCubemapSurface(sides[i], texture);
		}

		glTexParameteri(textureType, GL_TEXTURE_WRAP_R, textureWrappingMethod_R);
	}
	else if (textureType == GL_TEXTURE_2D)
	{
		w_stbi_set_flip_vertically_on_load(true);

		int w, h, garbageValue;
		unsigned char* textureData = w_stbi_load(textureAddress.data(), &w, &h, &garbageValue, 0);
		width = w;
		height = h;

		if (!textureData)
		{
			std::cerr << "Error loading the texture at file location : " << textureAddress << '\n';
			return;
		}

		glTexImage2D(textureType, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(textureType);
		w_stbi_image_free(textureData);
	}

	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, textureWrappingMethod_S);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, textureWrappingMethod_T);
	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, textureMinFilter);
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, textureMagFilter);
	glBindTexture(textureType, 0);
}

Texture::Texture(unsigned char* textureData, uint32_t width, uint32_t height, GLenum textureType, GLint textureWrappingMethod_S,
	GLint textureWrappingMethod_T, GLint textureWrappingMethod_R, GLint textureMinFilter, GLint textureMagFilter, GLint internalFormat,
	GLenum format) : textureType(textureType), textureID(0), width(width), height(height)
{
	glGenTextures(1, &textureID);
	glBindTexture(textureType, textureID);

	glTexImage2D(textureType, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(textureType);

	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, textureWrappingMethod_S);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, textureWrappingMethod_T);
	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, textureMinFilter);
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, textureMagFilter);
	glBindTexture(textureType, 0);
}

Texture::Texture(Texture&& texture) noexcept : textureID(texture.textureID), textureType(texture.textureType), width(texture.width),
	height(texture.height)
{
	texture.textureID = 0;
	texture.width = 0;
	texture.height = 0;
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
	texture.textureID = 0;
	texture.width = 0;
	texture.height = 0;
	return *this;
}

void Texture::useTexture(uint32_t textureUnit = 0) const
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(textureType, textureID);
}
