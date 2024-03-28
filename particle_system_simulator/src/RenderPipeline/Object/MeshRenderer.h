#pragma once

#include "RenderPipeline/Transform/Transformable.h"

class Mesh;
class Material;
class Shader;

class MeshRenderer : public Transformable
{
private:
	Mesh* mesh;
	Material* material;
	Shader* shader;

	static Shader* lastShader;
	static Material* lastMaterial;
	
	void useMaterial() const;
	void useShader() const;
public:
	MeshRenderer() = delete;
	MeshRenderer(Mesh* mesh, Material* material, Shader* shader = nullptr);
	MeshRenderer(const glm::vec3& position, Mesh* mesh, Material* material, Shader* shader = nullptr);
	MeshRenderer(const glm::vec3& position, const glm::vec3& rotation, Mesh* mesh, Material* material, Shader* shader = nullptr);
	MeshRenderer(const glm::vec3& position, const glm::quat& rotation, Mesh* mesh, Material* material, Shader* shader = nullptr);
	MeshRenderer(const Transform& transform, Mesh* mesh, Material* material, Shader* shader = nullptr);
	MeshRenderer(const MeshRenderer& object);
	~MeshRenderer();

	MeshRenderer& operator=(const MeshRenderer& object);

	void render();

	const Mesh* getMesh() const { return mesh; }
	Material* getMaterial() const { return material; }
	Shader* getShader() const { return shader; }

	void setMaterial(Material* material) { this->material = material; }
};
