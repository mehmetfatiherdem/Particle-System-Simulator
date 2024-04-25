#pragma once

#include <vector>
#include <functional>
#include <stdint.h>
#include "RenderPipeline/Shader/ShaderManagement/GlobalShaderManager.h"
#include "RenderPipeline/Light/LightManagement/LightTracker.h"
#include "RenderPipeline/Light/Data/LightDistance.h"
#include "RenderPipeline/Color/Color3.h"
#include "RenderPipeline/Camera/SceneCamera/SceneCamera.h"
#include "RenderPipeline/Skybox/Skybox.h"
#include "RenderPipeline/Transform/Data/TransformProps.h"

class DirectionalLight;
class PointLight;
class SpotLight;
class MeshRenderer;
class Mesh;
class Shader;
class Material;

class Scene
{
private:
	GlobalShaderManager shaderManager;		//Don't change the order of shaderManager and lightTracker because of their construction in the constructor
	LightTracker lightTracker;

	std::vector<LightSource*> lightSources;
	std::vector<MeshRenderer*> objects;

	SceneCamera camera;
	Skybox skybox;

	MeshRenderer* createObj(MeshRenderer* object);
	void destroyLight(LightSource* light, std::function<void()>&& untrack);

public:
	Scene() = delete;
	Scene(uint32_t windowWidth, uint32_t windowHeight);
	~Scene();

	void update();
	void render();
	
#pragma region Light Operations

	DirectionalLight* createDirectionalLight(const glm::vec3& direction, const Color3& color);
	DirectionalLight* createDirectionalLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);

	PointLight* createPointLight(const glm::vec3& position, const Color3& color, LightDistance distance);
	PointLight* createPointLight(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, LightDistance distance);
	PointLight* createPointLight(const glm::vec3& position, const Color3& color, float constant, float linear, float quadratic);
	PointLight* createPointLight(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float constant, float linear,
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

	LightSource* getLightAtIndex(uint32_t index) const { return lightSources[index]; }

	uint32_t numberOfLights() const { return lightSources.size(); }
	uint32_t numberOfDirectionalLights() const { return lightTracker.noOfDirLights; }
	uint32_t numberOfPointLights() const { return lightTracker.noOfPointLights; }
	uint32_t numberOfSpotLights() const { return lightTracker.noOfSpotLights; }

#pragma endregion

#pragma region Object Operations

	MeshRenderer* createObject(Mesh& mesh, Shader& shader, Material& material);
	MeshRenderer* createObject(const TransformProps& transform, Mesh& mesh, Shader& shader, Material& material);
	MeshRenderer* createObject(MeshRenderer* object);

	void destroyObject(MeshRenderer* object);
	uint32_t numberOfObjects() const { return objects.size(); }

#pragma endregion

	Camera& getCamera() { return camera; }
};
