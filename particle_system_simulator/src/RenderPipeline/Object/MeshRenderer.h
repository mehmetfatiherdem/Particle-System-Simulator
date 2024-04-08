#pragma once

#include <optional>
#include "RenderPipeline/Transform/Transformable.h"
#include "RenderPipeline/Mesh/Mesh.h"
#include "GeneralUtility/RefOrValue.h"

class Material;
class Shader;

class MeshRenderer : public Transformable
{
private:
	RefOrValue<Mesh> mesh;
	Shader& shader;
	Material* material;

	static Material* lastMaterial;

public:
	MeshRenderer() = delete;
	MeshRenderer(const TransformProps& transform, Mesh& mesh) : Transformable(transform), mesh(mesh, false),
		shader(Shader::genericShader()), material(&Material::defaultMaterial()) { }
	MeshRenderer(const TransformProps& transform, Mesh& mesh, Shader& shader);
	MeshRenderer(const TransformProps& transform, Mesh& mesh, Material& material);
	MeshRenderer(const TransformProps& transform, Mesh& mesh, Shader& shader, Material& material);
	MeshRenderer(const MeshRenderer&) = default;
	MeshRenderer(MeshRenderer&&) = default;
	~MeshRenderer();

	MeshRenderer& operator=(const MeshRenderer&) = default;
	MeshRenderer& operator=(MeshRenderer&&) = default;

	void render();

	/*const Mesh* getMesh() const { return mesh; }
	Material* getMaterial() const { return material; }
	Shader* getShader() const { return shader; }*/

	void setMaterial(Material* material) { this->material = material; }
};
