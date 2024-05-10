#include <glm/gtc/type_ptr.hpp>
#include <cassert>
#include "Data/BufferObjects.h"
#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<uint32_t>&& indices) : VAO(0), buffers(), props(std::nullopt),
vertices(std::make_shared<std::vector<Vertex>>(std::move(vertices))), updatedSinceLastDraw(0),
indices(std::make_shared<std::vector<uint32_t>>(std::move(indices)))
{

}

Mesh::Mesh(const Mesh& mesh) : VAO(mesh.VAO), buffers(), props(mesh.props), vertices(mesh.vertices), indices(mesh.indices),
instanceMatrices(mesh.instanceMatrices), updatedSinceLastDraw(mesh.updatedSinceLastDraw)
{
	if (!this->props.has_value())
		return;

	if (props->isInstanced)
	{
		std::memcpy(this->buffers, mesh.buffers, 3 * sizeof(this->buffers[0]));
	}
	else
	{
		initialize();
	}
}

Mesh::Mesh(Mesh&& mesh) noexcept : VAO(mesh.VAO), buffers(), props(mesh.props), vertices(std::move(mesh.vertices)),
indices(std::move(mesh.indices)), instanceMatrices(std::move(mesh.instanceMatrices)), updatedSinceLastDraw(mesh.updatedSinceLastDraw)
{
	if (!this->props.has_value())
		return;

	std::memcpy(this->buffers, mesh.buffers, 3 * sizeof(this->buffers[0]));
	std::fill_n(mesh.buffers, 3, 0);
	mesh.VAO = 0;
	mesh.props = std::nullopt;
}

Mesh& Mesh::operator=(const Mesh& mesh)
{
	cleanup();

	this->vertices = mesh.vertices;
	this->indices = mesh.indices;
	this->props = mesh.props;

	if (this->props.has_value())
	{
		if (this->props->isInstanced)
		{
			this->VAO = mesh.VAO;
			this->updatedSinceLastDraw = mesh.updatedSinceLastDraw;
			this->instanceMatrices = mesh.instanceMatrices;
			std::memcpy(this->buffers, mesh.buffers, 3 * sizeof(this->buffers[0]));
		}
		else
		{
			initialize();
		}
	}

	return *this;
}

Mesh& Mesh::operator=(Mesh&& mesh) noexcept
{
	cleanup();

	this->vertices = std::move(mesh.vertices);
	this->indices = std::move(mesh.indices);

	if (mesh.props.has_value())
	{
		this->VAO = mesh.VAO;
		this->props = mesh.props;
		this->updatedSinceLastDraw = mesh.updatedSinceLastDraw;
		this->instanceMatrices = std::move(mesh.instanceMatrices);
		std::memcpy(this->buffers, mesh.buffers, 3 * sizeof(this->buffers[0]));
		std::fill_n(mesh.buffers, 3, 0);
		mesh.VAO = 0;
		mesh.props = std::nullopt;
	}

	return *this;
}

Mesh::~Mesh()
{
	cleanup();
}

void Mesh::cleanup()
{
	if (!props.has_value())
		return;

	if (props->isInstanced && vertices.use_count() > 1)
		return;

	glDeleteBuffers(props->isInstanced ? 3 : 2, buffers);
	glDeleteVertexArrays(1, &VAO);
}

void Mesh::initialize()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(props->isInstanced ? 3 : 2, buffers);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[IBO]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof((*indices)[0]), &(*indices)[0], GL_STATIC_DRAW);

	uint32_t attribLocation = 0;

	glBindBuffer(GL_ARRAY_BUFFER, buffers[VBO]);
	glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(Vertex), &(*vertices)[0], GL_STATIC_DRAW);

	if (props->hasPosition)
	{
		glEnableVertexAttribArray(attribLocation);
		glVertexAttribPointer(attribLocation++, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	}

	if (props->hasNormal)
	{
		glEnableVertexAttribArray(attribLocation);
		glVertexAttribPointer(attribLocation++, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	}

	if (props->hasUV)
	{
		glEnableVertexAttribArray(attribLocation);
		glVertexAttribPointer(attribLocation++, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	}

	if (props->isInstanced)
	{
		uint32_t bufferSize = props->approximateCount ? props->approximateCount : 25;
		instanceMatrices = std::make_shared<std::vector<glm::mat4>>();
		instanceMatrices->reserve(bufferSize);
		instanceMatrices->push_back(glm::mat4(1.0f));

		glBindBuffer(GL_ARRAY_BUFFER, buffers[MBO]);
		glBufferData(GL_ARRAY_BUFFER, bufferSize * sizeof(glm::mat4), (void*)0, GL_STREAM_DRAW);

		for (uint32_t i = 0; i < 4; ++i)
		{
			glEnableVertexAttribArray(attribLocation);
			glVertexAttribPointer(attribLocation, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4)));
			glVertexAttribDivisor(attribLocation++, 1);
		}
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::initialize(VertexAttributes vertexAttribs)
{
	assert(!(this->props.has_value() && this->props->isInstanced && !vertexAttribs.isInstanced));
	cleanup();
	this->props = vertexAttribs;
	initialize();
}

void Mesh::draw() const
{
	glDepthMask(GL_FALSE);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
}

void Mesh::setModelMatrix(const glm::mat4& model)
{
	(*instanceMatrices)[updatedSinceLastDraw++] = model;

	if (updatedSinceLastDraw == instanceMatrices->size())
	{
		updatedSinceLastDraw = 0;

		glDepthMask(GL_FALSE);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[MBO]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, instanceMatrices->size() * sizeof(glm::mat4), &(*instanceMatrices)[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(VAO);
		glDrawElementsInstanced(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, 0, instanceMatrices->size());
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);
	}
}

void Mesh::addInstance()
{
	if (!props->isInstanced)
		return;

	size_t oldCapacity = instanceMatrices->capacity();
	instanceMatrices->push_back(glm::mat4(1.0f));

	if (instanceMatrices->capacity() == oldCapacity)
		return;

	glBindBuffer(GL_ARRAY_BUFFER, buffers[MBO]);
	glBufferData(GL_ARRAY_BUFFER, instanceMatrices->capacity() * sizeof(glm::mat4), (void*)0, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::removeInstance()
{
	if (props->isInstanced && !instanceMatrices->empty())
	{
		instanceMatrices->pop_back();
	}
}
