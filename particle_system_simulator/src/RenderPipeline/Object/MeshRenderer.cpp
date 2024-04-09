#include "RenderPipeline/Material/Material.h"
#include "RenderPipeline/Shader/Shader.h"
#include "MeshRenderer.h"

Material* MeshRenderer::lastMaterial = nullptr;

MeshRenderer::MeshRenderer(const TransformProps& transform, Mesh& mesh) : Transformable(transform), mesh(mesh, false),
	shader(&Shader::genericShader()), material(&Material::defaultMaterial())
{
	this->mesh->initialize(this->shader->getVertexAttributes());
}

MeshRenderer::MeshRenderer(const TransformProps& transform, Mesh& mesh, Shader& shader) : Transformable(transform), shader(&shader),
	mesh(mesh, shader.getVertexAttributes().isInstanced), material(&Material::defaultMaterial())
{
	this->mesh->initialize(this->shader->getVertexAttributes());
}

MeshRenderer::MeshRenderer(const TransformProps& transform, Mesh& mesh, Material& material) : Transformable(transform),
	shader(&Shader::genericShader()), mesh(mesh, this->shader->getVertexAttributes().isInstanced), material(&material)
{
	this->mesh->initialize(this->shader->getVertexAttributes());
}

MeshRenderer::MeshRenderer(const TransformProps& transform, Mesh& mesh, Shader& shader, Material& material) : Transformable(transform),
	shader(&shader), mesh(mesh, shader.getVertexAttributes().isInstanced), material(&material)
{
	this->mesh->initialize(this->shader->getVertexAttributes());
}

MeshRenderer::MeshRenderer(const MeshRenderer& mr) : Transformable(TransformProps{mr.transform}), shader(mr.shader), material(mr.material), mesh(mr.mesh)
{
	this->mesh->addInstance();
}

MeshRenderer::MeshRenderer(MeshRenderer&& mr) : Transformable(TransformProps{mr.transform}), shader(mr.shader), material(mr.material),
	mesh(std::move(mr.mesh)) { }

MeshRenderer::~MeshRenderer()
{
	if(mesh->isInstanced())
	{
		mesh->removeInstance();
	}
}

MeshRenderer& MeshRenderer::operator=(const MeshRenderer& mr)
{
	if(!mesh.isEmpty())
	{
		mesh->removeInstance();
	}

	shader = mr.shader;
	material = mr.material;
	mesh = mr.mesh;
	mesh->addInstance();
	return *this;
}

MeshRenderer& MeshRenderer::operator=(MeshRenderer&& mr)
{
	if(!mesh.isEmpty())
	{
		mesh->removeInstance();
	}

	shader = mr.shader;
	material = mr.material;
	mesh = std::move(mr.mesh);
	return *this;
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
