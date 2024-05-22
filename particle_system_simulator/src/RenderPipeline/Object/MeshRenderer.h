#pragma once

#include <functional>
#include "GeneralUtility/RefOrValue.h"
#include "RenderPipeline/Transform/Transformable.h"
#include "RenderPipeline/Mesh/Mesh.h"
#include "RenderPipeline/Material/Material.h"

class Shader;

class MeshRenderer : public Transformable
{
private:
	Material material;
	Shader* shader;
	RefOrValue<Mesh> mesh;
	std::function<void(Transform&)> preRenderAction;
	std::function<void()> postRenderAction;
	bool enabled;

public:
	MeshRenderer() = delete;
	MeshRenderer(const TransformProps& transform, Mesh& mesh);
	MeshRenderer(const TransformProps& transform, Mesh& mesh, Shader& shader);
	MeshRenderer(const TransformProps& transform, Mesh& mesh, const Material& material);
	MeshRenderer(const TransformProps& transform, Mesh& mesh, Shader& shader, const Material& material);
	MeshRenderer(const MeshRenderer& meshRenderer);
	MeshRenderer(MeshRenderer&& meshRenderer) noexcept;
	~MeshRenderer();

	MeshRenderer& operator=(const MeshRenderer& meshRenderer);
	MeshRenderer& operator=(MeshRenderer&& meshRenderer) noexcept;

	void render();

	void enable();
	void disable();
	void toggle();

	bool isEnabled() const { return enabled; }
	Material& getMaterial() { return material; }
	Shader* getShader() const { return shader; }

	void setMaterial(const Material& material) { this->material = material; }
	void setPreRenderAction(std::function<void(Transform&)> preRenderAction) { this->preRenderAction = preRenderAction; }
	void setPostRenderAction(std::function<void()> postRenderAction) { this->postRenderAction = postRenderAction; }
	void setShader(Shader& shader);
	void setEnabled(bool enabled) { this->enabled = enabled; }
};
