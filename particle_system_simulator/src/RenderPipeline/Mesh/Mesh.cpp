#include <glm/gtc/type_ptr.hpp>
#include "Data/BufferObjects.h"
#include "Mesh.h"

void Mesh::initializeMesh(const VertexAttributes& vertexAttribs)
{
	if(props.has_value())
	{
		cleanup();
		instanceMatrices->clear();
		instanceMatrices->shrink_to_fit();
	}
	
	this->props = vertexAttribs;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(props->isInstanced ? 3 : 2, buffers);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[IBO]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof((*indices)[0]), &(*indices)[0], GL_STATIC_DRAW);

	uint attribLocation = 0;

	glBindBuffer(GL_ARRAY_BUFFER, buffers[VBO]);
	glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(Vertex), &(*vertices)[0], GL_STATIC_DRAW);

	if(props->hasPosition)
	{
		glEnableVertexAttribArray(attribLocation);
		glVertexAttribPointer(attribLocation++, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	}

	if(props->hasNormal)
	{
		glEnableVertexAttribArray(attribLocation);
		glVertexAttribPointer(attribLocation++, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	}

	if(props->hasUV)
	{
		glEnableVertexAttribArray(attribLocation);
		glVertexAttribPointer(attribLocation++, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	}

	if(props->isInstanced)
	{
		uint bufferSize = props->approximateCount ? props->approximateCount : 25;
		instanceMatrices->reserve(bufferSize);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[MBO]);
		glBufferData(GL_ARRAY_BUFFER, bufferSize * sizeof(glm::mat4), (void*)0, GL_STREAM_DRAW);

		for(uint i = 0; i < 4; ++i)
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

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<uint>&& indices) : VAO(0), buffers(), props(std::nullopt),
	vertices(std::make_shared<std::vector<Vertex>>(std::move(vertices))), updatedSinceLastDraw(0),
	indices(std::make_shared<std::vector<uint>>(std::move(indices))), instanceMatrices(std::make_shared<std::vector<glm::mat4>>())
{

}

Mesh::Mesh(const Mesh& mesh) : vertices(mesh.vertices), indices(mesh.indices), instanceMatrices(mesh.instanceMatrices),
	updatedSinceLastDraw(mesh.updatedSinceLastDraw), buffers(), props(std::nullopt)
{
	if(!mesh.props.has_value())
		return;

	if(props->isInstanced)
	{
		this->VAO = mesh.VAO;
		this->props = mesh.props;
		std::memcpy(this->buffers, mesh.buffers, 3 * sizeof(this->buffers[0]));
	}
	else
	{
		initializeMesh(mesh.props.value());
	}
}

Mesh::Mesh(Mesh&& mesh) noexcept : VAO(mesh.VAO), props(mesh.props), updatedSinceLastDraw(mesh.updatedSinceLastDraw), buffers(),
	vertices(std::move(mesh.vertices)), indices(std::move(mesh.indices)), instanceMatrices(std::move(mesh.instanceMatrices))
{
	if(!this->props.has_value())
		return;

	std::memcpy(this->buffers, mesh.buffers, 3 * sizeof(this->buffers[0]));
	std::fill_n(mesh.buffers, 3, 0);
	mesh.props = std::nullopt;
	mesh.VAO = 0;
}

Mesh& Mesh::operator=(const Mesh& mesh)
{
	this->cleanup();

	this->vertices = mesh.vertices;
	this->indices = mesh.indices;

	if(mesh.props.has_value())
	{
		if(props->isInstanced)
		{
			this->VAO = mesh.VAO;
			this->props = mesh.props;
			this->updatedSinceLastDraw = mesh.updatedSinceLastDraw;
			this->instanceMatrices = mesh.instanceMatrices;
			std::memcpy(this->buffers, mesh.buffers, 3 * sizeof(this->buffers[0]));
		}
		else
		{
			initializeMesh(mesh.props.value());
		}
	}

	return *this;
}

Mesh& Mesh::operator=(Mesh&& mesh) noexcept
{
	this->cleanup();

	this->vertices = std::move(mesh.vertices);
	this->indices = std::move(mesh.indices);
	this->instanceMatrices = std::move(mesh.instanceMatrices);
	this->VAO = mesh.VAO;
	this->props = mesh.props;
	this->updatedSinceLastDraw = mesh.updatedSinceLastDraw;
	std::memcpy(this->buffers, mesh.buffers, 3 * sizeof(this->buffers[0]));
	std::fill_n(mesh.buffers, 3, 0);
	mesh.VAO = 0;

	return *this;
}

void Mesh::cleanup()
{
	if(props.has_value())
	{
		glDeleteBuffers(props->isInstanced ? 3 : 2, buffers);
		glDeleteVertexArrays(1, &VAO);
	}
}

Mesh::~Mesh()
{
	cleanup();
}

void Mesh::draw() const
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::setModelMatrix(const glm::mat4& model)
{
	(*instanceMatrices)[updatedSinceLastDraw++] = model;

	if(updatedSinceLastDraw == instanceMatrices->size())
	{
		updatedSinceLastDraw = 0;

		glBindBuffer(GL_ARRAY_BUFFER, buffers[MBO]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, instanceMatrices->size() * sizeof(glm::mat4), &(*instanceMatrices)[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(VAO);
		glDrawElementsInstanced(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, 0, instanceMatrices->size());
		glBindVertexArray(0);
	}
}

void Mesh::addInstance()
{
	if(!props.has_value() || !props->isInstanced)
		return;

	size_t oldCapacity = instanceMatrices->capacity();
	instanceMatrices->push_back(glm::mat4(1.0f));

	if(instanceMatrices->capacity() == oldCapacity)
		return;

	glBindBuffer(GL_ARRAY_BUFFER, buffers[MBO]);
	glBufferData(GL_ARRAY_BUFFER, instanceMatrices->capacity() * sizeof(glm::mat4), (void*)0, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::removeInstance()
{
	if(props.has_value() && props->isInstanced)
	{
		instanceMatrices->pop_back();
	}
}
