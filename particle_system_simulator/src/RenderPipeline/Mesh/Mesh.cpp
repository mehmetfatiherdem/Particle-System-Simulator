#include <cassert>
#include "Data/BufferObjects.h"
#include "Data/VertexAttributes.h"
#include "Data/MeshProperties.h"
#include "Mesh.h"

#define buffer_size (this->modelMatrices ? 3 : 2)

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<uint>&& indices, const MeshProperties& props) : vertices(std::move(vertices)),
	indices(std::move(indices)), VAO(0), buffers(), instanceCount(props.instanced ? 0 : 1), modelMatrices(nullptr),
	chunkSize(props.chunkSize), currentCapacity(props.estimatedNrOfInstances), updatedSinceLastDraw(0)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(props.instanced ? 3 : 2, buffers);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[IBO]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(this->indices[0]), &this->indices[0], GL_STATIC_DRAW);

	{
		glBindBuffer(GL_ARRAY_BUFFER, buffers[VBO]);
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(VERTEX_POSITION_LOCATION);
		glVertexAttribPointer(VERTEX_POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(VERTEX_NORMAL_LOCATION);
		glVertexAttribPointer(VERTEX_NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		glEnableVertexAttribArray(UV_LOCATION);
		glVertexAttribPointer(UV_LOCATION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	}

	if(props.instanced)
	{
		glBindBuffer(GL_ARRAY_BUFFER, buffers[MBO]);
		glBufferData(GL_ARRAY_BUFFER, currentCapacity * sizeof(glm::mat4), (void*)0, GL_STREAM_DRAW);

		for(unsigned int i = 0; i < 4; ++i)
		{
			glEnableVertexAttribArray(MODEL_MATRIX_LOCATION + i);
			glVertexAttribPointer(MODEL_MATRIX_LOCATION + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4)));
			glVertexAttribDivisor(MODEL_MATRIX_LOCATION + i, 1);
		}

		modelMatrices = new glm::mat4[currentCapacity];
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Mesh::Mesh(Mesh&& mesh) noexcept
	: VAO(mesh.VAO), buffers(), vertices(std::move(mesh.vertices)), indices(std::move(mesh.indices)), modelMatrices(mesh.modelMatrices),
	currentCapacity(mesh.currentCapacity), chunkSize(mesh.chunkSize), instanceCount(mesh.instanceCount), updatedSinceLastDraw(mesh.updatedSinceLastDraw)
{
	std::memcpy(this->buffers, mesh.buffers, buffer_size);
	std::fill_n(mesh.buffers, buffer_size, 0);
	mesh.VAO = 0;
	mesh.currentCapacity = 0;
	mesh.modelMatrices = nullptr;
}

Mesh::~Mesh()
{
	glDeleteBuffers(buffer_size, buffers);
	glDeleteVertexArrays(1, &VAO);
	if(modelMatrices)
		delete[] modelMatrices;
}

Mesh& Mesh::operator=(Mesh&& mesh) noexcept
{
	if(&mesh != this)
	{
		this->vertices = std::move(mesh.vertices);
		this->indices = std::move(mesh.indices);
		this->VAO = mesh.VAO;
		this->modelMatrices = mesh.modelMatrices;
		this->currentCapacity = mesh.currentCapacity;
		this->chunkSize = mesh.chunkSize;
		this->instanceCount = mesh.instanceCount;
		std::memcpy(this->buffers, mesh.buffers, buffer_size);
		std::fill_n(mesh.buffers, buffer_size, 0);
		mesh.VAO = 0;
		mesh.currentCapacity = 0;
		mesh.modelMatrices = nullptr;
	}

	return *this;
}

void Mesh::draw() const
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::setModelMatrix(const glm::mat4& model)
{
	modelMatrices[updatedSinceLastDraw++] = model;

	if(updatedSinceLastDraw == instanceCount)
	{
		updatedSinceLastDraw = 0;

		glBindBuffer(GL_ARRAY_BUFFER, buffers[MBO]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, instanceCount * sizeof(glm::mat4), modelMatrices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(VAO);
		glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, instanceCount);
		glBindVertexArray(0);
	}
}

void Mesh::addInstance()
{
	if(!modelMatrices)
		return;

	if(instanceCount++ < currentCapacity)
		return;

	currentCapacity += chunkSize;

	glm::mat4* newArr = new glm::mat4[currentCapacity];
	delete[] modelMatrices;
	modelMatrices = newArr;		//no need to copy the old memory since model matrix data is updated each frame

	glBindBuffer(GL_ARRAY_BUFFER, buffers[MBO]);
	glBufferData(GL_ARRAY_BUFFER, currentCapacity * sizeof(glm::mat4), (void*)0, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::removeInstance()
{
	if(modelMatrices)
	{
		--instanceCount;
	}
}

void Mesh::shrink(ShrinkMethod shrinkMethod)
{
	if(!modelMatrices)
		return;

	currentCapacity = instanceCount;

	if(shrinkMethod == ShrinkMethod::FitCurrentChunk)
	{
		currentCapacity += ((chunkSize - (instanceCount % chunkSize)) % chunkSize);
	}

	glm::mat4* newArr = nullptr;

	if(currentCapacity)
	{
		newArr = new glm::mat4[currentCapacity];
	}

	delete[] modelMatrices;
	modelMatrices = newArr;		//no need to copy the old memory since model matrix data is updated each frame

	glBindBuffer(GL_ARRAY_BUFFER, buffers[MBO]);
	glBufferData(GL_ARRAY_BUFFER, currentCapacity * sizeof(glm::mat4), (void*)0, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
