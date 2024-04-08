#include "RenderPipeline/Mesh/Mesh.h"
#include "RenderPipeline/Material/Material.h"
#include "MeshRenderer.h"
#include "RenderPipeline/Shader/Shader.h"

Material* MeshRenderer::lastMaterial = nullptr;

MeshRenderer::MeshRenderer(const TransformProps& transform, Mesh& mesh) : Transformable(transform), mesh(mesh, false),
	shader(Shader::genericShader()), material(&Material::defaultMaterial())
{
	mesh.initializeMesh(shader.getVertexAttributes());
}

MeshRenderer::~MeshRenderer()
{
	if(mesh->isInstanced())
	{
		mesh->removeInstance();
	}
}

MeshRenderer& MeshRenderer::operator=(const MeshRenderer& object)
{
	this->transform = object.transform;
	this->mesh = object.mesh;
	this->material = object.material;
	this->shader = object.shader;

	mesh->addInstance();
	return *this;
}

void MeshRenderer::render()
{
	if(shader.useShader() || material != lastMaterial)
	{
		lastMaterial = this->material;
		this->material->useMaterial(*shader);
	}

	if(mesh->isInstanced())
	{
		mesh->setModelMatrix(getModelMatrix());
	}
	else
	{
		shader->setMatrix4("model", getModelMatrix());
		mesh->draw();
	}
}
