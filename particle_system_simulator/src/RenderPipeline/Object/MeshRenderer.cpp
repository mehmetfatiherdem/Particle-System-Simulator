#include "RenderPipeline/Shader/Shader.h"
#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(const TransformProps& transform, Mesh& mesh)
	: Transformable(transform), mesh(mesh, false), shader(&Shader::genericShader()),
	material(Material::defaultMaterial()), enabled(true), preRenderAction([](Transform&) {}),
	postRenderAction([]() {})
{
	this->mesh->initializeOrAddInstance(this->shader->getVertexAttributes());
}

MeshRenderer::MeshRenderer(const TransformProps& transform, Mesh& mesh, Shader& shader)
	: Transformable(transform), mesh(mesh, shader.getVertexAttributes().isInstanced), shader(&shader),
	material(Material::defaultMaterial()), enabled(true), preRenderAction([](Transform&) {}), postRenderAction([]() {})
{
	this->mesh->initializeOrAddInstance(this->shader->getVertexAttributes());
}

MeshRenderer::MeshRenderer(const TransformProps& transform, Mesh& mesh, const Material& material)
	: Transformable(transform), mesh(mesh, false), material(material), shader(&Shader::genericShader()), enabled(true),
	preRenderAction([](Transform&) {}), postRenderAction([]() {})
{
	this->mesh->initializeOrAddInstance(this->shader->getVertexAttributes());
}

MeshRenderer::MeshRenderer(const TransformProps& transform, Mesh& mesh, Shader& shader, const Material& material)
	: Transformable(transform), mesh(mesh, shader.getVertexAttributes().isInstanced), material(material), shader(&shader),
	enabled(true), preRenderAction([](Transform&) {}), postRenderAction([]() {})
{
	this->mesh->initializeOrAddInstance(this->shader->getVertexAttributes());
}

MeshRenderer::MeshRenderer(const MeshRenderer& meshRenderer)
	: material(meshRenderer.material), shader(meshRenderer.shader), enabled(meshRenderer.enabled),
	preRenderAction(meshRenderer.preRenderAction), postRenderAction(meshRenderer.postRenderAction)
{
	MeshRenderer& constCasted = const_cast<MeshRenderer&>(meshRenderer);
	this->mesh = RefOrValue(*constCasted.mesh.get(), constCasted.mesh->isInstanced());
	this->mesh->addInstance();
}

MeshRenderer::MeshRenderer(MeshRenderer&& meshRenderer) noexcept
	: mesh(std::move(meshRenderer.mesh)), material(meshRenderer.material), shader(meshRenderer.shader), enabled(meshRenderer.enabled),
	preRenderAction(meshRenderer.preRenderAction), postRenderAction(meshRenderer.postRenderAction) { }

MeshRenderer::~MeshRenderer()
{
	if (enabled)
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

void MeshRenderer::enable()
{
	if (!enabled)
	{
		this->mesh->addInstance();
	}

	enabled = true;
}

void MeshRenderer::disable()
{
	if (enabled)
	{
		this->mesh->removeInstance();
	}

	enabled = false;
}

void MeshRenderer::toggle()
{
	if (enabled)
	{
		this->mesh->removeInstance();
	}
	else
	{
		this->mesh->addInstance();
	}

	enabled = !enabled;
}

void MeshRenderer::render()
{
	if (!enabled) return;

	shader->useShader();
	material.useMaterial(*shader);

	preRenderAction(transform);

	if (mesh->isInstanced())
	{
		mesh->setModelMatrix(getModelMatrix());
	}
	else
	{
		shader->setMatrix4("model", getModelMatrix());
		mesh->draw();
	}

	postRenderAction();
}

void MeshRenderer::setShader(Shader& shader)
{
	this->shader = &shader;
	this->mesh->initializeOrAddInstance(this->shader->getVertexAttributes());
}