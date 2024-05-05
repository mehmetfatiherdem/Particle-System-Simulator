#include <glm/gtc/type_ptr.hpp>
#include "RenderPipeline/Light/Utility/GLSLObjectUtils.h"
#include "RenderPipeline/Material/MaterialGLSL.h"
#include "RenderPipeline/Shader/Data/UBOConstants.h"
#include "RenderPipeline/Shader/Data/MatrixPair.h"
#include "GlobalShaderManager.h"

GlobalShaderManager::GlobalShaderManager() : UBO_Global_Matrices(0), UBO_Lights(0), UBO_View(0), UBO_Material(0)
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

	glGenBuffers(1, &UBO_Material);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO_Material);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(MaterialGLSL), (void*)0, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, MATERIAL_UNIFORM_BLOCK_INDEX, UBO_Material);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GlobalShaderManager::editLightsUBO(uint32_t lightOffset, uint32_t lightDataSize, const void* lightData, uint32_t numOffset, const void* numData) const
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
	MatrixPair pair{view, projection};

	glBindBuffer(GL_UNIFORM_BUFFER, UBO_Global_Matrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(pair), &pair);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GlobalShaderManager::updateMaterial(const MaterialGLSL& material) const
{
	glBindBuffer(GL_UNIFORM_BUFFER, UBO_Material);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(MaterialGLSL), &material);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GlobalShaderManager::addDirectionalLight(const DirectionalLight& light, uint32_t newLength) const
{
	DirectionalLightGLSL glslLight = getGLSLRepresentation(light);
	uint32_t lightDataOffset = calculateLightDataOffset(LightSourceType::DirectionalLight, newLength - 1);
	uint32_t numberDataOffset = calculateNumberDataOffset(LightSourceType::DirectionalLight);
	editLightsUBO(lightDataOffset, sizeof(DirectionalLightGLSL), &glslLight, numberDataOffset, &newLength);
}

void GlobalShaderManager::addPointLight(const PointLight& light, uint32_t newLength) const
{
	PointLightGLSL glslLight = getGLSLRepresentation(light);
	uint32_t lightDataOffset = calculateLightDataOffset(LightSourceType::PointLight, newLength - 1);
	uint32_t numberDataOffset = calculateNumberDataOffset(LightSourceType::PointLight);
	editLightsUBO(lightDataOffset, sizeof(PointLightGLSL), &glslLight, numberDataOffset, &newLength);
}

void GlobalShaderManager::addSpotLight(const SpotLight& light, uint32_t newLength) const
{
	SpotLightGLSL glslLight = getGLSLRepresentation(light);
	uint32_t lightDataOffset = calculateLightDataOffset(LightSourceType::SpotLight, newLength - 1);
	uint32_t numberDataOffset = calculateNumberDataOffset(LightSourceType::SpotLight);
	editLightsUBO(lightDataOffset, sizeof(SpotLightGLSL), &glslLight, numberDataOffset, &newLength);
}

void GlobalShaderManager::updateDirectionalLight(const DirectionalLight& light, uint32_t index) const
{
	DirectionalLightGLSL glslLight = getGLSLRepresentation(light);
	uint32_t lightDataOffset = calculateLightDataOffset(LightSourceType::DirectionalLight, index);
	editLightsUBO(lightDataOffset, sizeof(DirectionalLightGLSL), &glslLight, 0, nullptr);
}

void GlobalShaderManager::updatePointLight(const PointLight& light, uint32_t index) const
{
	PointLightGLSL glslLight = getGLSLRepresentation(light);
	uint32_t lightDataOffset = calculateLightDataOffset(LightSourceType::PointLight, index);
	editLightsUBO(lightDataOffset, sizeof(PointLightGLSL), &glslLight, 0, nullptr);
}

void GlobalShaderManager::updateSpotLight(const SpotLight& light, uint32_t index) const
{
	SpotLightGLSL glslLight = getGLSLRepresentation(light);
	uint32_t lightDataOffset = calculateLightDataOffset(LightSourceType::SpotLight, index);
	editLightsUBO(lightDataOffset, sizeof(SpotLightGLSL), &glslLight, 0, nullptr);
}

void GlobalShaderManager::removeDirectionalLight(const DirectionalLight* replacementLight, uint32_t indexToRemove, uint32_t newLength) const
{
	DirectionalLightGLSL glslLight;
	uint32_t lightDataOffset = 0;

	if(replacementLight != nullptr)
	{
		glslLight = getGLSLRepresentation(*replacementLight);
		lightDataOffset = calculateLightDataOffset(LightSourceType::DirectionalLight, indexToRemove);
	}

	uint32_t numberDataOffset = calculateNumberDataOffset(LightSourceType::DirectionalLight);
	editLightsUBO(lightDataOffset, sizeof(DirectionalLightGLSL), &glslLight, numberDataOffset, &newLength);
}

void GlobalShaderManager::removePointLight(const PointLight* replacementLight, uint32_t indexToRemove, uint32_t newLength) const
{
	PointLightGLSL glslLight;
	uint32_t lightDataOffset = 0;

	if(replacementLight != nullptr)
	{
		glslLight = getGLSLRepresentation(*replacementLight);
		lightDataOffset = calculateLightDataOffset(LightSourceType::PointLight, indexToRemove);
	}

	uint32_t numberDataOffset = calculateNumberDataOffset(LightSourceType::PointLight);
	editLightsUBO(lightDataOffset, sizeof(PointLightGLSL), &glslLight, numberDataOffset, &newLength);
}

void GlobalShaderManager::removeSpotLight(const SpotLight* replacementLight, uint32_t indexToRemove, uint32_t newLength) const
{
	SpotLightGLSL glslLight;
	uint32_t lightDataOffset = 0;

	if(replacementLight != nullptr)
	{
		glslLight = getGLSLRepresentation(*replacementLight);
		lightDataOffset = calculateLightDataOffset(LightSourceType::SpotLight, indexToRemove);
	}

	uint32_t numberDataOffset = calculateNumberDataOffset(LightSourceType::SpotLight);
	editLightsUBO(lightDataOffset, sizeof(SpotLightGLSL), &glslLight, numberDataOffset, &newLength);
}
