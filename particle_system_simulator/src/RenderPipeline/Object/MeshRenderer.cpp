#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(const Mesh* mesh, Material* material) : mesh(mesh), material(material)
{
	assert(mesh != nullptr && "Mesh can't be null!");
	assert(material != nullptr && "Material can't be null!");
}

void MeshRenderer::render(const glm::mat4& model) const
{
	material->useMaterial(model);
	mesh->draw();
}