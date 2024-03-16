#include <glm/gtc/type_ptr.hpp>
#include "RenderPipeline/Light/LightUtility/GLSLObjectUtils.h"
#include "GlobalShaderManager.h"

GlobalShaderManager::GlobalShaderManager() : UBO_Global_Matrices(0), UBO_Lights(0), UBO_View(0)
{
	glGenBuffers(1, &UBO_Global_Matrices);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO_Global_Matrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), (void*)0, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, MATRICES_UNIFORM_BLOCK_INDEX, UBO_Global_Matrices);

	glGenBuffers(1, &UBO_Lights);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO_Lights);
	glBufferData(GL_UNIFORM_BUFFER, sizeOfLightsUBO(), (void*)0, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, LIGHTS_UNIFORM_BLOCK_INDEX, UBO_Lights);

	glGenBuffers(1, &UBO_View);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO_View);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), (void*)0, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, VIEW_UNIFORM_BLOCK_INDEX, UBO_View);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GlobalShaderManager::editLightsUBO(uint lightOffset, uint lightDataSize, const void* lightData, uint numOffset, const void* numData) const
{
	glBindBuffer(GL_UNIFORM_BUFFER, UBO_Lights);

	if(lightData != nullptr)
		glBufferSubData(GL_UNIFORM_BUFFER, lightOffset, lightDataSize, lightData);

	if(numData != nullptr)
		glBufferSubData(GL_UNIFORM_BUFFER, numOffset, sizeof(float), numData);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GlobalShaderManager::updateViewPosition(const glm::vec3& viewPosition) const
{
	glBindBuffer(GL_UNIFORM_BUFFER, UBO_View);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(viewPosition));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GlobalShaderManager::updateViewMatrix(const glm::mat4& view) const
{
	glBindBuffer(GL_UNIFORM_BUFFER, UBO_Global_Matrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GlobalShaderManager::updateProjectionMatrix(const glm::mat4& projection) const
{
	glBindBuffer(GL_UNIFORM_BUFFER, UBO_Global_Matrices);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GlobalShaderManager::updateViewProjectionMatrices(const glm::mat4& view, const glm::mat4& projection) const
{
	glBindBuffer(GL_UNIFORM_BUFFER, UBO_Global_Matrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GlobalShaderManager::addDirectionalLight(const DirectionalLight& light, uint newLength) const
{
	DirectionalLightToBytes glslLight = getGLSLRepresentation(light);
	uint lightDataOffset = calculateLightDataOffset(LightSourceType::DirectionalLight, newLength - 1);
	uint numberDataOffset = calculateNumberDataOffset(LightSourceType::DirectionalLight);
	editLightsUBO(lightDataOffset, sizeof(DirectionalLightGLSL), glslLight.bytes, numberDataOffset, &newLength);
}

void GlobalShaderManager::addPointLight(const PointLight& light, uint newLength) const
{
	PointLightToBytes glslLight = getGLSLRepresentation(light);
	uint lightDataOffset = calculateLightDataOffset(LightSourceType::PointLight, newLength - 1);
	uint numberDataOffset = calculateNumberDataOffset(LightSourceType::PointLight);
	editLightsUBO(lightDataOffset, sizeof(PointLightGLSL), glslLight.bytes, numberDataOffset, &newLength);
}

void GlobalShaderManager::addSpotLight(const SpotLight& light, uint newLength) const
{
	SpotLightToBytes glslLight = getGLSLRepresentation(light);
	uint lightDataOffset = calculateLightDataOffset(LightSourceType::SpotLight, newLength - 1);
	uint numberDataOffset = calculateNumberDataOffset(LightSourceType::SpotLight);
	editLightsUBO(lightDataOffset, sizeof(SpotLightGLSL), glslLight.bytes, numberDataOffset, &newLength);
}

void GlobalShaderManager::updateDirectionalLight(const DirectionalLight& light, uint index) const
{
	DirectionalLightToBytes glslLight = getGLSLRepresentation(light);
	uint lightDataOffset = calculateLightDataOffset(LightSourceType::DirectionalLight, index);
	editLightsUBO(lightDataOffset, sizeof(DirectionalLightGLSL), glslLight.bytes, 0, nullptr);
}

void GlobalShaderManager::updatePointLight(const PointLight& light, uint index) const
{
	PointLightToBytes glslLight = getGLSLRepresentation(light);
	uint lightDataOffset = calculateLightDataOffset(LightSourceType::PointLight, index);
	editLightsUBO(lightDataOffset, sizeof(PointLightGLSL), glslLight.bytes, 0, nullptr);
}

void GlobalShaderManager::updateSpotLight(const SpotLight& light, uint index) const
{
	SpotLightToBytes glslLight = getGLSLRepresentation(light);
	uint lightDataOffset = calculateLightDataOffset(LightSourceType::SpotLight, index);
	editLightsUBO(lightDataOffset, sizeof(SpotLightGLSL), glslLight.bytes, 0, nullptr);
}

void GlobalShaderManager::removeDirectionalLight(const DirectionalLight* replacementLight, uint indexToRemove, uint newLength) const
{
	DirectionalLightToBytes glslLight;
	uint lightDataOffset = 0;
	void* lightData = nullptr;

	if(replacementLight != nullptr)
	{
		glslLight = getGLSLRepresentation(*replacementLight);
		lightData = glslLight.bytes;
		lightDataOffset = calculateLightDataOffset(LightSourceType::DirectionalLight, indexToRemove);
	}

	uint numberDataOffset = calculateNumberDataOffset(LightSourceType::DirectionalLight);
	editLightsUBO(lightDataOffset, sizeof(DirectionalLightGLSL), lightData, numberDataOffset, &newLength);
}

void GlobalShaderManager::removePointLight(const PointLight* replacementLight, uint indexToRemove, uint newLength) const
{
	PointLightToBytes glslLight;
	uint lightDataOffset = 0;
	void* lightData = nullptr;

	if(replacementLight != nullptr)
	{
		glslLight = getGLSLRepresentation(*replacementLight);
		lightData = glslLight.bytes;
		lightDataOffset = calculateLightDataOffset(LightSourceType::PointLight, indexToRemove);
	}

	uint numberDataOffset = calculateNumberDataOffset(LightSourceType::PointLight);
	editLightsUBO(lightDataOffset, sizeof(PointLightGLSL), lightData, numberDataOffset, &newLength);
}

void GlobalShaderManager::removeSpotLight(const SpotLight* replacementLight, uint indexToRemove, uint newLength) const
{
	SpotLightToBytes glslLight;
	uint lightDataOffset = 0;
	void* lightData = nullptr;

	if(replacementLight != nullptr)
	{
		glslLight = getGLSLRepresentation(*replacementLight);
		lightData = glslLight.bytes;
		lightDataOffset = calculateLightDataOffset(LightSourceType::SpotLight, indexToRemove);
	}

	uint numberDataOffset = calculateNumberDataOffset(LightSourceType::SpotLight);
	editLightsUBO(lightDataOffset, sizeof(SpotLightGLSL), lightData, numberDataOffset, &newLength);
}
