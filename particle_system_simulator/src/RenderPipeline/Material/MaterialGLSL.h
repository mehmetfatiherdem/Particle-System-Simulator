#pragma once

#include <glm/vec4.hpp>
#include "stdint.h"

#pragma pack(push, 1)

struct MaterialGLSL
{
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	float shininess;
	uint32_t useDiffuseMap;
	uint32_t useSpecularMap;
	float padding1;
};

#pragma pack(pop)