#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "GeneralUtility/gl2fw3.h"
#include "RenderPipeline/Light/Data/LightConstants.h"

class DirectionalLight;
class PointLight;
class SpotLight;

class GlobalShaderManager
{
private:
	friend class Scene;

	GlobalShaderManager();

	GLuint UBO_Global_Matrices;
	GLuint UBO_Lights;
	GLuint UBO_View;

	void editLightsUBO(uint32_t lightOffset, uint32_t lightDataSize, const void* lightData, uint32_t numOffset, const void* numData) const;

public:
	GlobalShaderManager(const GlobalShaderManager& globalShaderManager) = delete;
	GlobalShaderManager(GlobalShaderManager&& globalShaderManager) = delete;

	GlobalShaderManager& operator=(const GlobalShaderManager& globalShaderManager) = delete;
	GlobalShaderManager& operator=(GlobalShaderManager&& globalShaderManager) = delete;

	void updateViewPosition(const glm::vec3& viewPosition) const;
	void updateViewMatrix(const glm::mat4& view) const;
	void updateProjectionMatrix(const glm::mat4& projection) const;
	void updateViewProjectionMatrices(const glm::mat4& view, const glm::mat4& projection) const;

	void addDirectionalLight(const DirectionalLight& light, uint32_t newLength) const;
	void addPointLight(const PointLight& light, uint32_t newLength) const;
	void addSpotLight(const SpotLight& light, uint32_t newLength) const;

	void updateDirectionalLight(const DirectionalLight& light, uint32_t index) const;
	void updatePointLight(const PointLight& light, uint32_t index) const;
	void updateSpotLight(const SpotLight& light, uint32_t index) const;

	void removeDirectionalLight(const DirectionalLight* replacementLight, uint32_t indexToRemove, uint32_t newLength) const;
	void removePointLight(const PointLight* replacementLight, uint32_t indexToRemove, uint32_t newLength) const;
	void removeSpotLight(const SpotLight* replacementLight, uint32_t indexToRemove, uint32_t newLength) const;
};
