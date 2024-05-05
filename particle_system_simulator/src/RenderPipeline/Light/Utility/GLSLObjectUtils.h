#pragma once

#include <stdint.h>
#include <glm/vec3.hpp>
#include "LightSourceType.h"

#pragma pack(push, 1)			//Disable padding for these primitives, so that all the padding is done manually for GLSL

struct DirectionalLightGLSL
{
	glm::vec3 ambient;
	float padding1;
	glm::vec3 diffuse;
	float padding2;
	glm::vec3 specular;
	float padding3;
	glm::vec3 direction;
	float padding4;
};

struct PointLightGLSL
{
	glm::vec3 ambient;
	float padding1;
	glm::vec3 diffuse;
	float padding2;
	glm::vec3 specular;
	float padding3;
	glm::vec3 position;
	float padding4;
	float constant;
	float linear;
	float quadratic;
	float padding5;
};

struct SpotLightGLSL
{
	glm::vec3 ambient;
	float padding1;
	glm::vec3 diffuse;
	float padding2;
	glm::vec3 specular;
	float padding3;
	glm::vec3 position;
	float padding4;
	glm::vec3 direction;
	float padding5;
	float constant;
	float linear;
	float quadratic;
	float padding6;
	float innerCutOff;
	float outerCutOff;
	float epsilon;
	float padding7;
};

#pragma pack(pop)				//Enable default padding again

class DirectionalLight;
class PointLight;
class SpotLight;

uint32_t sizeOfLightsUBO();

uint32_t calculateNumberDataOffset(LightSourceType type);
uint32_t calculateLightDataOffset(LightSourceType type, uint32_t index);

DirectionalLightGLSL getGLSLRepresentation(const DirectionalLight& light);
PointLightGLSL getGLSLRepresentation(const PointLight& light);
SpotLightGLSL getGLSLRepresentation(const SpotLight& light);
