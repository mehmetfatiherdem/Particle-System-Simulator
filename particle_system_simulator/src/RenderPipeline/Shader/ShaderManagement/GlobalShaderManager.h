#pragma once

#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "GeneralUtility/Aliases.h"
#include "RenderPipeline/Constants/PipelineConstants.h"

using namespace constants::light;
using namespace constants::UBO;

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

	void editLightsUBO(uint lightOffset, uint lightDataSize, const void* lightData, uint numOffset, const void* numData) const;

public:
	GlobalShaderManager(const GlobalShaderManager& globalShaderManager) = delete;
	GlobalShaderManager(GlobalShaderManager&& globalShaderManager) = delete;

	GlobalShaderManager& operator=(const GlobalShaderManager& globalShaderManager) = delete;
	GlobalShaderManager& operator=(GlobalShaderManager&& globalShaderManager) = delete;

	void updateViewPosition(const glm::vec3& viewPosition) const;
	void updateViewMatrix(const glm::mat4& view) const;
	void updateProjectionMatrix(const glm::mat4& projection) const;
	void updateViewProjectionMatrices(const glm::mat4& view, const glm::mat4& projection) const;

	void addDirectionalLight(const DirectionalLight& light, uint newLength) const;
	void addPointLight(const PointLight& light, uint newLength) const;
	void addSpotLight(const SpotLight& light, uint newLength) const;

	void updateDirectionalLight(const DirectionalLight& light, uint index) const;
	void updatePointLight(const PointLight& light, uint index) const;
	void updateSpotLight(const SpotLight& light, uint index) const;

	void removeDirectionalLight(const DirectionalLight* replacementLight, uint indexToRemove, uint newLength) const;
	void removePointLight(const PointLight* replacementLight, uint indexToRemove, uint newLength) const;
	void removeSpotLight(const SpotLight* replacementLight, uint indexToRemove, uint newLength) const;
};
