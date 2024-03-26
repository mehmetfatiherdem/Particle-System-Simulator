#include "RenderPipeline/Mesh/Mesh.h"
#include "RenderPipeline/Material/Material.h"
#include "RenderableObject.h"

RenderableObject::RenderableObject(Mesh* mesh, Material* material) : Transformable(), mesh(mesh), material(material) 
{
	mesh->addInstance();
}

RenderableObject::RenderableObject(const glm::vec3& position, Mesh* mesh, Material* material) : Transformable(position), mesh(mesh), material(material) 
{
	mesh->addInstance();
}

RenderableObject::RenderableObject(const glm::vec3& position, const glm::vec3& rotation, Mesh* mesh, Material* material) : Transformable(position, rotation),
	mesh(mesh), material(material) 
{ 
	mesh->addInstance();
}

RenderableObject::RenderableObject(const glm::vec3& position, const glm::quat& rotation, Mesh* mesh, Material* material) : Transformable(position, rotation),
	mesh(mesh), material(material)
{
	mesh->addInstance();
}

RenderableObject::RenderableObject(const Transform& transform, Mesh* mesh, Material* material) : Transformable(transform), mesh(mesh), material(material)
{
	mesh->addInstance();
}

RenderableObject::RenderableObject(const RenderableObject& renderableObject) : Transformable(renderableObject.transform),
	mesh(renderableObject.mesh), material(renderableObject.material)
{
	mesh->addInstance();
}

RenderableObject& RenderableObject::operator=(const RenderableObject& renderableObject)
{
	transform = renderableObject.transform;
	mesh = renderableObject.mesh;
	material = renderableObject.material;

	mesh->addInstance();
	return *this;
}

RenderableObject::~RenderableObject()
{
	mesh->removeInstance();

	if(!mesh->isInstanced())
		delete mesh;
}

void RenderableObject::render()
{
	if(mesh->isInstanced())
	{
		material->useMaterial(getModelMatrix());
		mesh->setModelMatrix(getModelMatrix());		//handle material better
	}
	else
	{
		material->useMaterial(getModelMatrix());
		mesh->draw();
	}
}