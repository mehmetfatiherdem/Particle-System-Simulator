#pragma once

#include <glm/mat4x4.hpp>
#include "RenderPipeline/Mesh/Mesh.h"
#include "RenderPipeline/Material/Material.h"

class MeshRenderer
{
private:
	const Mesh* mesh;
	Material* material;
public:
	MeshRenderer() = delete;
	MeshRenderer(const Mesh* mesh, Material* material);

	void render(const glm::mat4& model) const;

	const Mesh* getMesh() const { return mesh; }
	Material* getMaterial() const { return material; }

	void setMaterial(Material* material) { this->material = material; }
};