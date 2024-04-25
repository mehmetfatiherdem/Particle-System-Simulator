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
	MeshRenderer(const MeshRenderer& meshRenderer);
	MeshRenderer(MeshRenderer&& meshRenderer) noexcept;
	~MeshRenderer();

	MeshRenderer& operator=(const MeshRenderer& meshRenderer);
	MeshRenderer& operator=(MeshRenderer&& meshRenderer) noexcept;

	void render();

	Material* getMaterial() const { return material; }
	Shader* getShader() const { return shader; }

	void setMaterial(Material& material) { this->material = &material; }
	void setShader(Shader& shader);
};
