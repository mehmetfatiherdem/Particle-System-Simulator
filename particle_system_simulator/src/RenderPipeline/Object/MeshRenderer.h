#pragma once

#include "RenderPipeline/Transform/Transformable.h"
#include "RenderPipeline/Mesh/Mesh.h"
#include "GeneralUtility/RefOrValue.h"

class Material;
class Shader;

class MeshRenderer : public Transformable
{
private:
	Shader* shader;
	Material* material;
	RefOrValue<Mesh> mesh;

	static Material* lastMaterial;

public:
	MeshRenderer() = delete;
	MeshRenderer(const TransformProps& transform, Mesh& mesh);
	MeshRenderer(const TransformProps& transform, Mesh& mesh, Shader& shader);
	MeshRenderer(const TransformProps& transform, Mesh& mesh, Material& material);
	MeshRenderer(const TransformProps& transform, Mesh& mesh, Shader& shader, Material& material);
	MeshRenderer(const MeshRenderer& mr);
	MeshRenderer(MeshRenderer&& mr);
	~MeshRenderer();

	MeshRenderer& operator=(const MeshRenderer& mr);
	MeshRenderer& operator=(MeshRenderer&& mr);

	void render();

	Shader& getShader() { return *shader; }
	Material& getMaterial() { return *material; }
	
	void setMaterial(Material& material) { this->material = &material; }
	void setShader(Shader& shader);
};
