#include "RenderPipeline/Shader/Shader.h"
#include "RenderPipeline/Material/Material.h"
#include "MeshRenderer.h"

Material* MeshRenderer::lastMaterial = nullptr;

MeshRenderer::MeshRenderer(Mesh& mesh)
	: mesh(mesh, false), material(&Material::defaultMaterial()), shader(&Shader::genericShader())
{
	this->mesh->initialize(this->shader->getVertexAttributes());
}

MeshRenderer::MeshRenderer(Mesh& mesh, Shader& shader)
	: mesh(mesh, false), material(&Material::defaultMaterial()), shader(&shader)
{
	this->mesh->initialize(this->shader->getVertexAttributes());
}

MeshRenderer::MeshRenderer(Mesh& mesh, Material& material)
	: mesh(mesh, false), material(&material), shader(&Shader::genericShader())
{
	this->mesh->initialize(this->shader->getVertexAttributes());
}

MeshRenderer::MeshRenderer(Mesh& mesh, Shader& shader, Material& material)
	: mesh(mesh, false), material(&material), shader(&shader)
{
	this->mesh->initialize(this->shader->getVertexAttributes());
}

MeshRenderer::~MeshRenderer()
{
	if (mesh->isInstanced())
	{
		mesh->removeInstance();
	}
}

void MeshRenderer::render()
{
	if(shader->useShader() || material != lastMaterial)
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
