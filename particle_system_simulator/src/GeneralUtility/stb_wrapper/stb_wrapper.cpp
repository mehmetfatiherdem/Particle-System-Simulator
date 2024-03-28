#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "stb_wrapper.h"

unsigned char* w_stbi_load(const char* fileName, int* width, int* height, int* fileChannels, int desiredChannels)
{
	return stbi_load(fileName, width, height, fileChannels, desiredChannels);
}

void w_stbi_image_free(void* data)
{
	stbi_image_free(data);
}

void w_stbi_set_flip_vertically_on_load(int flip)
{
	stbi_set_flip_vertically_on_load(flip);
}