#pragma once

#include "RenderPipeline/Transform/Transform.h"
#include "MeshRenderer.h"

class RenderableObject
{
private:
	Transform transform;
	MeshRenderer meshRenderer;
public:
	RenderableObject(const Transform& transform, const MeshRenderer& meshRenderer) : transform(transform), meshRenderer(meshRenderer) { }

	Transform& getTransform() { return transform; }
	void render() const { meshRenderer.render(transform.getModelMatrix()); }
};
