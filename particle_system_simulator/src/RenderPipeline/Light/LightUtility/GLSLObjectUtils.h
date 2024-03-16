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

union DirectionalLightToBytes
{
	DirectionalLightGLSL directionalLight;
	signed char bytes[sizeof(DirectionalLightGLSL)];
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

union PointLightToBytes
{
	PointLightGLSL pointLight;
	signed char bytes[sizeof(PointLightGLSL)];
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

union SpotLightToBytes
{
	SpotLightGLSL spotLight;
	signed char bytes[sizeof(SpotLightGLSL)];
};

#pragma pack(pop)				//Enable default padding again

class DirectionalLight;
class PointLight;
class SpotLight;

unsigned int sizeOfLightsUBO();

unsigned int calculateNumberDataOffset(LightSourceType type);
unsigned int calculateLightDataOffset(LightSourceType type, unsigned int index);

DirectionalLightToBytes getGLSLRepresentation(const DirectionalLight& light);
PointLightToBytes getGLSLRepresentation(const PointLight& light);
SpotLightToBytes getGLSLRepresentation(const SpotLight& light);
