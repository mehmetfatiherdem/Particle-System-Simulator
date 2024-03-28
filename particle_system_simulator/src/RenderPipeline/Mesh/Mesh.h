#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include "GeneralUtility/Aliases.h"
#include "Data/Vertex.h"
#include "Data/ShrinkMethod.h"

struct MeshProperties;

class Mesh
{
private:
	friend class MeshRenderer;

	GLuint VAO;
	GLuint buffers[3];

	std::vector<Vertex> vertices;
	std::vector<uint> indices;
	glm::mat4* modelMatrices;
	uint chunkSize;
	uint currentCapacity;
	uint instanceCount;
	uint updatedSinceLastDraw;

	void draw() const;
	void setModelMatrix(const glm::mat4& model);
	void addInstance();
	void removeInstance();
	void shrink(ShrinkMethod shrinkMethod);

public:
	Mesh() = delete;
	Mesh(std::vector<Vertex>&& vertices, std::vector<uint>&& indices, const MeshProperties& props);
	Mesh(const Mesh& mesh) = delete;
	Mesh(Mesh&& mesh) noexcept;
	~Mesh();

	Mesh& operator=(const Mesh& mesh) = delete;
	Mesh& operator=(Mesh&& mesh) noexcept;

	GLsizei getVertexCount() const { return vertices.size(); }
	GLsizei getIndexCount() const { return indices.size(); }
	Vertex getVertexAt(uint i) const { return vertices[i]; }
	uint getIndexAt(uint i) const { return indices[i]; }

	bool isInstanced() const { return modelMatrices != nullptr; }
	uint getInstanceCount() const { return instanceCount; }
};