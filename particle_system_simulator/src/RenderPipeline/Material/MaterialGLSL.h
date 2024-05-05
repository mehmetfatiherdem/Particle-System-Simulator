#pragma once

#include <glm/vec4.hpp>
#include "stdint.h"

#pragma pack(push, 1)

struct MaterialGLSL
{
	glm::vec3 ambient;
	float padding1;
	glm::vec3 diffuse;
	float padding2;
	glm::vec3 specular;
	float padding3;
	float alpha;
	float shininess;
	uint32_t useDiffuseMap;
	uint32_t useSpecularMap;
};

#pragma pack(pop)