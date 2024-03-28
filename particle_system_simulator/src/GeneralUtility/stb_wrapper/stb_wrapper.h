#pragma once

unsigned char* w_stbi_load(const char* fileName, int* width, int* height, int* fileChannels, int desiredChannels);
void w_stbi_image_free(void* data);
void w_stbi_set_flip_vertically_on_load(int flip);