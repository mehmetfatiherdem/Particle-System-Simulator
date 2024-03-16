#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices, GLenum usage)
	: vertices(std::move(vertices)), indices(std::move(indices)), VAO(0), VBO(0), IBO(0)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], usage);

	glGenBuffers(1, &IBO);	//Generate Index buffer object in the GPU
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);	//Bind Index buffer to the VAO
	//Connect the buffer data to the indices for indexed drawing to prevent using duplicate vertices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(this->indices[0]), &this->indices[0], usage);

	glEnableVertexAttribArray(0);	//Enable vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);	//Enable normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glBindVertexArray(0);	//Always unbind the VAO first
	glBindBuffer(GL_ARRAY_BUFFER, 0);	//Unbind the vertex buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	//Unbind Index buffer
}

Mesh::Mesh(Mesh&& mesh) noexcept
	: VAO(mesh.VAO), VBO(mesh.VBO), IBO(mesh.IBO), vertices(std::move(mesh.vertices)),
	indices(std::move(mesh.indices))
{
	mesh.VAO = mesh.VBO = mesh.IBO = 0;
}

Mesh::~Mesh()
{
	if(IBO != 0)
		glDeleteBuffers(1, &IBO);	//Delete index buffer from the GPU memory
	if(VBO != 0)
		glDeleteBuffers(1, &VBO);	//Delete vertex buffer from the GPU memory
	if(VAO != 0)
		glDeleteVertexArrays(1, &VAO);	//Delete the vertex array from the GPU memory

	VAO = VBO = IBO = 0;
}

Mesh& Mesh::operator=(Mesh&& mesh) noexcept
{
	if(&mesh != this)
	{
		this->vertices = std::move(mesh.vertices);
		this->indices = std::move(mesh.indices);
		this->VAO = mesh.VAO;
		this->VBO = mesh.VBO;
		this->IBO = mesh.IBO;

		mesh.VAO = mesh.VBO = mesh.IBO = 0;
	}

	return *this;
}

void Mesh::draw() const
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
