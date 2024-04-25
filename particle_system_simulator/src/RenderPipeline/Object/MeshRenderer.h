#pragma once

#include "GeneralUtility/RefOrValue.h"
#include "RenderPipeline/Transform/Transformable.h"
#include "RenderPipeline/Mesh/Mesh.h"

class Material;
class Shader;

class MeshRenderer : public Transformable
{
private:
	Material* material;
	Shader* shader;
	RefOrValue<Mesh> mesh;

	static Material* lastMaterial;
	
public:
	MeshRenderer() = delete;
	MeshRenderer(Mesh& mesh);
	MeshRenderer(Mesh& mesh, Shader& shader);
	MeshRenderer(Mesh& mesh, Material& material);
	MeshRenderer(Mesh& mesh, Shader& shader, Material& material);
	~MeshRenderer();

	//TODO : Implement the copy and move constructors as well


	/*MeshRenderer(const glm::vec3& position, Mesh* mesh, Material* material, Shader* shader = nullptr);
	MeshRenderer(const glm::vec3& position, const glm::vec3& rotation, Mesh* mesh, Material* material, Shader* shader = nullptr);
	MeshRenderer(const glm::vec3& position, const glm::quat& rotation, Mesh* mesh, Material* material, Shader* shader = nullptr);
	MeshRenderer(const Transform& transform, Mesh* mesh, Material* material, Shader* shader = nullptr);*/

	void render();

	Material* getMaterial() const { return material; }
	Shader* getShader() const { return shader; }

	void setMaterial(Material& material) { this->material = &material; }
	//TODO : void setShader(Shader& shader);
};
