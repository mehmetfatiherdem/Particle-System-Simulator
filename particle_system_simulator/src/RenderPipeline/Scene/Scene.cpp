#include "RenderPipeline/Light/DirectionalLight.h"
#include "RenderPipeline/Light/PointLight.h"
#include "RenderPipeline/Light/SpotLight.h"
#include "RenderPipeline/Light/Data/LightConstants.h"
#include "RenderPipeline/Object/MeshRenderer.h"
#include "RenderPipeline/Object/MeshRendererSorter.h"
#include "RenderPipeline/Mesh/Mesh.h"
#include "RenderPipeline/Material/Material.h"
#include "RenderPipeline/Shader/Shader.h"
#include "Scene.h"

Scene::Scene(float aspectRatio) : shaderManager(), lightTracker(this->shaderManager), camera(glm::vec3{0.0f, 0.0f, -10.0f}, aspectRatio),
	lightSources(MAX_DIRECTIONAL_LIGHTS + MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS), objects() { }

Scene::~Scene()
{
	for (auto light : lightSources)
	{
		delete light;
	}
	
	for (auto object : objects)
	{
		delete object;
	}
}

void Scene::render()
{
	shaderManager.updateViewProjectionMatrices(camera.getViewMatrix(), camera.getProjectionMatrix());
	shaderManager.updateViewPosition(camera.getTransform().getPosition());

	for(auto object : objects)
	{
		object->render();
	}

	//render skybox
}

#pragma region Light Operations

DirectionalLight* Scene::createDirectionalLight(const glm::vec3& direction, const Color3& color)
{
	if(!lightTracker.canAddDirectionalLight())
		return nullptr;

	DirectionalLight* light = new DirectionalLight(lightTracker, color, direction);
	lightSources.push_back(light);
	lightTracker.trackLight(light);
	
	return light;
}

DirectionalLight* Scene::createDirectionalLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
{
	if(!lightTracker.canAddDirectionalLight())
		return nullptr;

	DirectionalLight* light = new DirectionalLight(lightTracker, Color3{ambient, diffuse, specular}, direction);
	lightSources.push_back(light);
	lightTracker.trackLight(light);
	return light;
}

PointLight* Scene::createPointLight(const glm::vec3& position, const Color3& color, LightDistance distance)
{
	if(!lightTracker.canAddPointLight())
		return nullptr;

	PointLight* light = new PointLight(lightTracker, position, color, distance);
	lightSources.push_back(light);
	lightTracker.trackLight(light);
	return light;
}

PointLight* Scene::createPointLight(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, LightDistance distance)
{
	if(!lightTracker.canAddPointLight())
		return nullptr;

	PointLight* light = new PointLight(lightTracker, position, Color3{ambient, diffuse, specular}, distance);
	lightSources.push_back(light);
	lightTracker.trackLight(light);
	return light;
}

PointLight* Scene::createPointLight(const glm::vec3& position, const Color3& color, float constant, float linear, float quadratic)
{
	if(!lightTracker.canAddPointLight())
		return nullptr;

	PointLight* light = new PointLight(lightTracker, position, color, constant, linear, quadratic);
	lightSources.push_back(light);
	lightTracker.trackLight(light);
	return light;
}

PointLight* Scene::createPointLight(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float constant,
	float linear, float quadratic)
{
	if(!lightTracker.canAddPointLight())
		return nullptr;

	PointLight* light = new PointLight(lightTracker, position, Color3{ambient, diffuse, specular}, constant, linear, quadratic);
	lightSources.push_back(light);
	lightTracker.trackLight(light);
	return light;
}

SpotLight* Scene::createSpotLight(const glm::vec3& position, const glm::vec3& direction, const Color3& color, LightDistance distance, float innerCutOffAngle,
	float outerCutOffAngle)
{
	if(!lightTracker.canAddSpotLight())
		return nullptr;

	SpotLight* light = new SpotLight(lightTracker, position, direction, color, innerCutOffAngle, outerCutOffAngle, distance);
	lightSources.push_back(light);
	lightTracker.trackLight(light);
	return light;
}

SpotLight* Scene::createSpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
	LightDistance distance, float innerCutOffAngle, float outerCutOffAngle)
{
	if(!lightTracker.canAddSpotLight())
		return nullptr;

	SpotLight* light = new SpotLight(lightTracker, position, direction, Color3{ambient, diffuse, specular}, innerCutOffAngle, outerCutOffAngle, distance);
	lightSources.push_back(light);
	lightTracker.trackLight(light);
	return light;
}

SpotLight* Scene::createSpotLight(const glm::vec3& position, const glm::vec3& direction, const Color3& color, float constant, float linear, float quadratic,
	float innerCutOffAngle, float outerCutOffAngle)
{
	if(!lightTracker.canAddSpotLight())
		return nullptr;

	SpotLight* light = new SpotLight(lightTracker, position, direction, color, innerCutOffAngle, outerCutOffAngle, constant, linear, quadratic);
	lightSources.push_back(light);
	lightTracker.trackLight(light);
	return light;
}

SpotLight* Scene::createSpotLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
	float constant, float linear, float quadratic, float innerCutOffAngle, float outerCutOffAngle)
{
	if(!lightTracker.canAddSpotLight())
		return nullptr;

	SpotLight* light = new SpotLight(lightTracker, position, direction, Color3{ambient, diffuse, specular}, innerCutOffAngle, outerCutOffAngle, constant, linear, quadratic);
	lightSources.push_back(light);
	lightTracker.trackLight(light);
	return light;
}

void Scene::destroyLight(LightSource* light, std::function<void()>&& untrack)
{
	auto end = lightSources.end();
	auto iterator = std::find(lightSources.begin(), end, light);

	if(iterator != end)
	{
		untrack();
		lightSources.erase(iterator);
		delete light;
	}
}

void Scene::destroyLight(LightSource* light)
{
	if(DirectionalLight* dirLight = dynamic_cast<DirectionalLight*>(light))
		destroyLight(dirLight);
	else if(PointLight* pointLight = dynamic_cast<PointLight*>(light))
		destroyLight(pointLight);
	else
		destroyLight(dynamic_cast<SpotLight*>(light));
}

void Scene::destroyLight(DirectionalLight* light)
{
	destroyLight(light, [this, &light]() {
		this->lightTracker.untrackLight(light);
		});
}

void Scene::destroyLight(PointLight* light)
{
	destroyLight(light, [this, &light]() {
		this->lightTracker.untrackLight(light);
		});
}

void Scene::destroyLight(SpotLight* light)
{
	destroyLight(light, [this, &light]() {
		this->lightTracker.untrackLight(light);
		});
}

#pragma endregion

#pragma region Object Operations

MeshRenderer* Scene::createObject(MeshRenderer* object)
{
	bool inserted = false;

	for(size_t i = 0; i < objects.size(); ++i)
	{
		if(!compare(*objects[i], *object))
		{
			objects.insert(objects.begin() + i, object);
			inserted = true;
			break;
		}
	}

	if(!inserted)
	{
		objects.push_back(object);
	}

	return object;
}

void Scene::destroyObject(MeshRenderer* object)
{
	auto end = objects.end();
	auto found = std::find(objects.begin(), end, object);

	if(found != end)
	{
		objects.erase(found);
		delete object;
	}
}

MeshRenderer* Scene::createObject(Mesh* mesh, Material* material, Shader* shader)
{
	return createObject(new MeshRenderer{mesh, material, shader});
}

MeshRenderer* Scene::createObject(const glm::vec3& position, Mesh* mesh, Material* material, Shader* shader)
{
	return createObject(new MeshRenderer{position, mesh, material, shader});
}

MeshRenderer* Scene::createObject(const glm::vec3& position, const glm::vec3& rotation, Mesh* mesh, Material* material, Shader* shader)
{
	return createObject(new MeshRenderer{position, rotation, mesh, material, shader});
}

MeshRenderer* Scene::createObject(const glm::vec3& position, const glm::quat& rotation, Mesh* mesh, Material* material, Shader* shader)
{
	return createObject(new MeshRenderer{position, rotation, mesh, material, shader});
}

MeshRenderer* Scene::createObject(const Transform& transform, Mesh* mesh, Material* material, Shader* shader)
{
	return createObject(new MeshRenderer{transform, mesh, material, shader});
}

#pragma endregion