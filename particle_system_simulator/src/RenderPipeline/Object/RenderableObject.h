#pragma once

#include "RenderPipeline/Transform/Transformable.h"

class Mesh;
class Material;

class RenderableObject : public Transformable
{
private:
	Mesh* mesh;
	Material* material;

public:
	RenderableObject() = delete;
	RenderableObject(Mesh* mesh, Material* material);
	RenderableObject(const glm::vec3& position, Mesh* mesh, Material* material);
	RenderableObject(const glm::vec3& position, const glm::vec3& rotation, Mesh* mesh, Material* material);
	RenderableObject(const glm::vec3& position, const glm::quat& rotation, Mesh* mesh, Material* material);
	RenderableObject(const Transform& transform, Mesh* mesh, Material* material);
	RenderableObject(const RenderableObject& renderableObject);
	~RenderableObject();

	RenderableObject& operator=(const RenderableObject& renderableObject);

	void render();

	const Mesh* getMesh() const { return mesh; }
	Material* getMaterial() const { return material; }

	void setMaterial(Material* material) { this->material = material; }
};
