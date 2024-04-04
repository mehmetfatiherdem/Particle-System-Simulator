#pragma once

#include <vector>
#include <functional>
#include "RenderPipeline/Shader/ShaderManagement/GlobalShaderManager.h"
#include "RenderPipeline/Light/LightManagement/LightTracker.h"
#include "RenderPipeline/Light/Data/LightDistance.h"
#include "RenderPipeline/Color/Color3.h"
#include "RenderPipeline/Camera/SceneCamera/SceneCamera.h"
#include "RenderPipeline/Skybox/Skybox.h"

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

	MeshRenderer* createObject(MeshRenderer* object);
	void destroyLight(LightSource* light, std::function<void()>&& untrack);

public:
	Scene() = delete;
	Scene(unsigned int windowWidth, unsigned int windowHeight);
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

	LightSource* getLightAtIndex(unsigned int index) const { return lightSources[index]; }

	unsigned int numberOfLights() const { return lightSources.size(); }
	unsigned int numberOfDirectionalLights() const { return lightTracker.noOfDirLights; }
	unsigned int numberOfPointLights() const { return lightTracker.noOfPointLights; }
	unsigned int numberOfSpotLights() const { return lightTracker.noOfSpotLights; }

#pragma endregion

#pragma region Object Operations

	MeshRenderer* createObject(Mesh* mesh, Material* material, Shader* shader = nullptr);
	MeshRenderer* createObject(const glm::vec3& position, Mesh* mesh, Material* material, Shader* shader = nullptr);
	MeshRenderer* createObject(const glm::vec3& position, const glm::vec3& rotation, Mesh* mesh, Material* material, Shader* shader = nullptr);
	MeshRenderer* createObject(const glm::vec3& position, const glm::quat& rotation, Mesh* mesh, Material* material, Shader* shader = nullptr);
	MeshRenderer* createObject(const Transform& transform, Mesh* mesh, Material* material, Shader* shader = nullptr);

	void destroyObject(MeshRenderer* object);
	unsigned int numberOfObjects() const { return objects.size(); }

#pragma endregion

	Camera& getCamera() { return camera; }
};
