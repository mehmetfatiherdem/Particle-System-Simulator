#include "RenderPipeline/Shader/Shader.h"
#include "RenderPipeline/Material/Material.h"
#include "MeshRenderer.h"

Material* MeshRenderer::lastMaterial = nullptr;

MeshRenderer::MeshRenderer(const TransformProps& transform, Mesh& mesh)
	: Transformable(transform), mesh(mesh, false), shader(&Shader::genericShader()), material(&Material::defaultMaterial()),
	enabled(true)
{
	this->mesh->initialize(this->shader->getVertexAttributes());
}

MeshRenderer::MeshRenderer(const TransformProps& transform, Mesh& mesh, Shader& shader)
	: Transformable(transform), mesh(mesh, shader.getVertexAttributes().isInstanced), shader(&shader),
	material(&Material::defaultMaterial()), enabled(true)
{
	this->mesh->initialize(this->shader->getVertexAttributes());
}

MeshRenderer::MeshRenderer(const TransformProps& transform, Mesh& mesh, Material& material)
	: Transformable(transform), mesh(mesh, false), material(&material), shader(&Shader::genericShader()), enabled(true)
{
	this->mesh->initialize(this->shader->getVertexAttributes());
}

MeshRenderer::MeshRenderer(const TransformProps& transform, Mesh& mesh, Shader& shader, Material& material)
	: Transformable(transform), mesh(mesh, shader.getVertexAttributes().isInstanced), material(&material), shader(&shader),
	enabled(true)
{
	this->mesh->initialize(this->shader->getVertexAttributes());
}

MeshRenderer::MeshRenderer(const MeshRenderer& meshRenderer)
	: material(meshRenderer.material), shader(meshRenderer.shader), enabled(meshRenderer.enabled)
{
	MeshRenderer& constCasted = const_cast<MeshRenderer&>(meshRenderer);
	this->mesh = RefOrValue(*constCasted.mesh.get(), constCasted.mesh->isInstanced());
	this->mesh->addInstance();
}

MeshRenderer::MeshRenderer(MeshRenderer&& meshRenderer) noexcept
	: mesh(std::move(meshRenderer.mesh)), material(meshRenderer.material), shader(meshRenderer.shader), enabled(meshRenderer.enabled)
{}

MeshRenderer::~MeshRenderer()
{
	mesh->removeInstance();
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
	this->enabled = meshRenderer.enabled;
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
	this->enabled = meshRenderer.enabled;

	return *this;
}

void MeshRenderer::render()
{
	if (!enabled) return;

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