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

MeshRenderer::MeshRenderer(const MeshRenderer& meshRenderer)
	: material(meshRenderer.material), shader(meshRenderer.shader)
{
	MeshRenderer& constCasted = const_cast<MeshRenderer&>(meshRenderer);
	this->mesh = RefOrValue(*constCasted.mesh.get(), constCasted.mesh->isInstanced());
	this->mesh->addInstance();
}

MeshRenderer::MeshRenderer(MeshRenderer&& meshRenderer) noexcept
	: mesh(std::move(meshRenderer.mesh)), material(meshRenderer.material), shader(meshRenderer.shader)
{}

MeshRenderer::~MeshRenderer()
{
	if (mesh->isInstanced())
	{
		mesh->removeInstance();
	}
}

MeshRenderer& MeshRenderer::operator=(const MeshRenderer& meshRenderer)
{
	if (!this->mesh.isEmpty())
	{
		this->mesh->removeInstance();
	}

	this->mesh = meshRenderer.mesh;
	this->material = meshRenderer.material;
	this->shader = meshRenderer.shader;
	this->mesh->addInstance();

	return *this;
}

MeshRenderer& MeshRenderer::operator=(MeshRenderer&& meshRenderer) noexcept
{
	if (!this->mesh.isEmpty())
	{
		this->mesh->removeInstance();
	}

	this->mesh = std::move(meshRenderer.mesh);
	this->material = meshRenderer.material;
	this->shader = meshRenderer.shader;

	return *this;
}

void MeshRenderer::render()
{
	if (shader->useShader() || material != lastMaterial)
	{
		lastMaterial = this->material;
		this->material->useMaterial(*shader);
	}

	if (mesh->isInstanced())
	{
		mesh->setModelMatrix(getModelMatrix());
	}
	else
	{
		shader->setMatrix4("model", getModelMatrix());
		mesh->draw();
	}
}

void MeshRenderer::setShader(Shader& shader)
{
	this->shader = &shader;
	this->mesh->initialize(this->shader->getVertexAttributes());
}