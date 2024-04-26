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
	bool enabled;

	static Material* lastMaterial;
	
public:
	MeshRenderer() = delete;
	MeshRenderer(const TransformProps& transform, Mesh& mesh);
	MeshRenderer(const TransformProps& transform, Mesh& mesh, Shader& shader);
	MeshRenderer(const TransformProps& transform, Mesh& mesh, Material& material);
	MeshRenderer(const TransformProps& transform, Mesh& mesh, Shader& shader, Material& material);
	MeshRenderer(const MeshRenderer& meshRenderer);
	MeshRenderer(MeshRenderer&& meshRenderer) noexcept;
	~MeshRenderer();

	MeshRenderer& operator=(const MeshRenderer& meshRenderer);
	MeshRenderer& operator=(MeshRenderer&& meshRenderer) noexcept;

	void render();

	void enable() { enabled = true; }
	void disable() { enabled = false; }
	void toggle() { enabled = !enabled; }

	bool isEnabled() const { return enabled; }
	Material* getMaterial() const { return material; }
	Shader* getShader() const { return shader; }

	void setMaterial(Material& material) { this->material = &material; }
	void setShader(Shader& shader);
	void setEnabled(bool enabled) { this->enabled = enabled; }
};
