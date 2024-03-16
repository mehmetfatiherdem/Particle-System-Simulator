#pragma once

#include <GL/glew.h>
#include <vector>
#include "Vertex.h"

class Vector;

class Mesh
{
private:
	GLuint VAO, VBO, IBO;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

public:
	Mesh() = delete;
	Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices, GLenum usage);
	Mesh(const Mesh& mesh) = delete;
	Mesh(Mesh&& mesh) noexcept;
	~Mesh();

	Mesh& operator=(const Mesh& mesh) = delete;
	Mesh& operator=(Mesh&& mesh) noexcept;

	void draw() const;

	GLsizei getIndexCount() const { return indices.size(); }
	GLsizei getVertexCount() const { return vertices.size(); }
	Vertex getVertexAt(unsigned int i) const { return vertices[i]; }
	unsigned int getIndexAt(unsigned int i) const { return indices[i]; }
};