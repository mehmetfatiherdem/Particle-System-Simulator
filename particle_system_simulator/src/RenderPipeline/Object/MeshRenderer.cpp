#include "RenderPipeline/Mesh/Mesh.h"
#include "RenderPipeline/Material/Material.h"
#include "MeshRenderer.h"
#include "RenderPipeline/Shader/Shader.h"

Shader* MeshRenderer::lastShader = nullptr;
Material* MeshRenderer::lastMaterial = nullptr;

MeshRenderer::MeshRenderer(Mesh* mesh, Material* material, Shader* shader) : Transformable(), mesh(mesh), material(material),
shader(shader != nullptr ? shader : (mesh->isInstanced() ? &Shader::getInstancedShader() : &Shader::getGenericShader()))
{
	mesh->addInstance();
}

MeshRenderer::MeshRenderer(const glm::vec3& position, Mesh* mesh, Material* material, Shader* shader) : Transformable(position), mesh(mesh),
material(material), shader(shader != nullptr ? shader : (mesh->isInstanced() ? &Shader::getInstancedShader() : &Shader::getGenericShader()))
{
	mesh->addInstance();
}

MeshRenderer::MeshRenderer(const glm::vec3& position, const glm::vec3& rotation, Mesh* mesh, Material* material, Shader* shader) :
	Transformable(position, rotation), mesh(mesh), material(material),
	shader(shader != nullptr ? shader : (mesh->isInstanced() ? &Shader::getInstancedShader() : &Shader::getGenericShader()))
{
	mesh->addInstance();
}

MeshRenderer::MeshRenderer(const glm::vec3& position, const glm::quat& rotation, Mesh* mesh, Material* material, Shader* shader) :
	Transformable(position, rotation), mesh(mesh), material(material),
	shader(shader != nullptr ? shader : (mesh->isInstanced() ? &Shader::getInstancedShader() : &Shader::getGenericShader()))
{
	mesh->addInstance();
}

MeshRenderer::MeshRenderer(const Transform& transform, Mesh* mesh, Material* material, Shader* shader) : Transformable(transform), mesh(mesh),
material(material), shader(shader != nullptr ? shader : (mesh->isInstanced() ? &Shader::getInstancedShader() : &Shader::getGenericShader()))
{
	mesh->addInstance();
}

MeshRenderer::MeshRenderer(const MeshRenderer& object) : Transformable(object.transform),
mesh(object.mesh), material(object.material), shader(object.shader)
{
	mesh->addInstance();
}

MeshRenderer::~MeshRenderer()
{
	if(mesh->isInstanced())
	{
		mesh->removeInstance();
	}
	else
	{
		delete mesh;
	}
}

MeshRenderer& MeshRenderer::operator=(const MeshRenderer& object)
{
	this->transform = object.transform;
	this->mesh = object.mesh;
	this->material = object.material;
	this->shader = object.shader;

	mesh->addInstance();
	return *this;
}

void MeshRenderer::render()
{
	if(shader->useShader() || material != lastMaterial)
	{
		lastMaterial = this->material;
		this->material->useMaterial(*shader);
	}

	if(mesh->isInstanced())
	{
		mesh->setModelMatrix(getModelMatrix());
	}
	else
	{
		shader->setMatrix4("model", getModelMatrix());
		mesh->draw();
	}
}
