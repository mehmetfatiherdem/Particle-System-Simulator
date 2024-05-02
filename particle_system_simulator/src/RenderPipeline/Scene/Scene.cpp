#include <functional>
#include <algorithm>
#include "RenderPipeline/Light/DirectionalLight.h"
#include "RenderPipeline/Light/PointLight.h"
#include "RenderPipeline/Light/SpotLight.h"
#include "RenderPipeline/Light/Data/LightConstants.h"
#include "RenderPipeline/Object/MeshRenderer.h"
#include "RenderPipeline/Mesh/Mesh.h"
#include "RenderPipeline/Material/Material.h"
#include "RenderPipeline/Shader/Shader.h"
#include "GeneralUtility/stringify.h"
#include "GeneralUtility/MathConstants.h"
#include "Scene.h"

std::string getTextureAddresses()
{
	return "Resources/Textures/Skybox/right.jpg,Resources/Textures/Skybox/left.jpg,Resources/Textures/Skybox/top.jpg,Resources/Textures/Skybox/bottom.jpg,Resources/Textures/Skybox/front.jpg,Resources/Textures/Skybox/back.jpg";
}

Scene::Scene(uint32_t windowWidth, uint32_t windowHeight) : shaderManager(), lightTracker(this->shaderManager),
	camera(glm::vec3{0.0f, 0.0f, 10.0f}, windowWidth, windowHeight), skybox(getTextureAddresses()),
	lightSources(MAX_DIRECTIONAL_LIGHTS + MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS), objects() {}

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

void Scene::update()
{
	camera.update();
}

void Scene::render()
{
	shaderManager.updateViewProjectionMatrices(camera.getViewMatrix(), camera.getProjectionMatrix());
	shaderManager.updateViewPosition(camera.getTransform().getPosition());

	//Render opaque objects first
	std::for_each(objects.begin(), objects.end(), [](MeshRenderer* object)
		{
			if (std::abs(object->getMaterial().getAlpha() - 1.0f) < EPSILON)
			{
				object->render();
			}
		});

	//Sort the objects based on their distance from the camera (from farthest to nearest), also stack all the opaque objects
	//at the back of the vector since they're rendered but still inside the vector.
#pragma region Premature Optimization Possibility
	/*std::sort uses quicksort, which is O(n ^ 2) in its worst case which happens when the array is already sorted or almost sorted.
	The array will most likely be sorted or almost sorted due to it getting sorted each frame.
	So, the worst case will most likely happen most frames.
	You might want to apply a hybrid of insertion sort and quicksort to avoid the worst case.
	Let's be real though, you won't do that. You're too lazy.*/
#pragma endregion

	//The saying of the day: true comes before false.

	std::sort(objects.begin(), objects.end(), [&](MeshRenderer* obj1, MeshRenderer* obj2)
		{
			if (std::abs(obj1->getMaterial().getAlpha() - 1.0f) < EPSILON) return false;
			if (std::abs(obj2->getMaterial().getAlpha() - 1.0f) < EPSILON) return true;

			float obj1Distance = glm::length(obj1->getTransform().getPosition() - camera.getTransform().getPosition());
			float obj2Distance = glm::length(obj2->getTransform().getPosition() - camera.getTransform().getPosition());

			return obj1Distance > obj2Distance;
		});

	for (auto object : objects)
	{
		if (std::abs(object->getMaterial().getAlpha() - 1.0f) < EPSILON) break;
		object->render();
	}

	skybox.render(camera.getViewMatrix(), camera.getProjectionMatrix(45.0f));
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

MeshRenderer* Scene::createObject(const TransformProps& transform, Mesh& mesh)
{
	MeshRenderer* obj = new MeshRenderer(transform, mesh);
	objects.push_back(obj);
	return obj;
}

MeshRenderer* Scene::createObject(const TransformProps& transform, Mesh& mesh, Shader& shader)
{
	MeshRenderer* obj = new MeshRenderer(transform, mesh, shader);
	objects.push_back(obj);
	return obj;
}

MeshRenderer* Scene::createObject(const TransformProps& transform, Mesh& mesh, const Material& material)
{
	MeshRenderer* obj = new MeshRenderer(transform, mesh, material);
	objects.push_back(obj);
	return obj;
}

MeshRenderer* Scene::createObject(const TransformProps& transform, Mesh& mesh, Shader& shader, const Material& material)
{
	MeshRenderer* obj = new MeshRenderer(transform, mesh, shader, material);
	objects.push_back(obj);
	return obj;
}

MeshRenderer* Scene::createObject(MeshRenderer* object)
{
	MeshRenderer* obj = new MeshRenderer(*object);
	objects.push_back(obj);
	return obj;
}

#pragma endregion
