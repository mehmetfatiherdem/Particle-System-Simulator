#pragma once

#include <vector>
#include <functional>
#include "RenderPipeline/Shader/ShaderManagement/GlobalShaderManager.h"
#include "RenderPipeline/Light/LightManagement/LightTracker.h"
#include "RenderPipeline/Light/Enums/LightDistance.h"
#include "RenderPipeline/Color/Color3.h"
#include "RenderPipeline/Camera/Camera.h"

class DirectionalLight;
class PointLight;
class SpotLight;

class Scene
{
private:
	GlobalShaderManager shaderManager;
	LightTracker lightTracker;

	Camera camera;

	std::vector<LightSource*> lightSources;

	void deleteLight(LightSource* light, std::function<void()>&& untrack);

public:
	Scene() = delete;
	Scene(float aspectRatio);
	~Scene();

	void update();

	DirectionalLight* createDirectionalLight(const glm::vec3& direction, const Color3& color);
	DirectionalLight* createDirectionalLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);

	PointLight* createPointLight(const glm::vec3& position, const Color3& color, LightDistance distance);
	PointLight* createPointLight(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, LightDistance distance);
	PointLight* createPointLight(const glm::vec3& position, const Color3& color, float constant, float linear, float quadratic);
	PointLight* createPointLight(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float constant,float linear,
		float quadratic);

	SpotLight* createSpotLight(const glm::vec3& position, const glm::vec3& direction, const Color3& color, LightDistance distance, float innerCutOffAngle, float outerCutOffAngle);
	SpotLight* createSpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
		LightDistance distance, float innerCutOffAngle, float outerCutOffAngle);
	SpotLight* createSpotLight(const glm::vec3& position, const glm::vec3& direction, const Color3& color, float constant, float linear, float quadratic, float innerCutOffAngle,
		float outerCutOffAngle);
	SpotLight* createSpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
		float constant, float linear, float quadratic, float innerCutOffAngle, float outerCutOffAngle);

	void destroyLight(LightSource* light);
	void destroyLight(DirectionalLight* light);
	void destroyLight(PointLight* light);
	void destroyLight(SpotLight* light);

	unsigned int numberOfLights() const { return lightSources.size(); }
	unsigned int numberOfDirectionalLights() const { return lightTracker.noOfDirLights; }
	unsigned int numberOfPointLights() const { return lightTracker.noOfPointLights; }
	unsigned int numberOfSpotLights() const { return lightTracker.noOfSpotLights; }

	LightSource* getLightAtIndex(unsigned int index) const { return lightSources[index]; }

	Camera& getCamera() { return camera; }
};
