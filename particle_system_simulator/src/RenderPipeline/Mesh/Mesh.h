#pragma once

#include <vector>
#include <stdint.h>
#include <glm/mat4x4.hpp>
#include "GeneralUtility/gl2fw3.h"
#include "Data/Vertex.h"
#include "Data/ShrinkMethod.h"

struct MeshProperties;

class Mesh
{
private:
	friend class MeshRenderer;
	friend class Skybox;

	GLuint VAO;
	GLuint buffers[3];

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	glm::mat4* modelMatrices;
	uint32_t chunkSize;
	uint32_t currentCapacity;
	uint32_t instanceCount;
	uint32_t updatedSinceLastDraw;

	void draw() const;
	void setModelMatrix(const glm::mat4& model);
	void addInstance();
	void removeInstance();
	void shrink(ShrinkMethod shrinkMethod);

public:
	Mesh() = delete;
	Mesh(std::vector<Vertex>&& vertices, std::vector<uint32_t>&& indices, const MeshProperties& props);
	Mesh(const Mesh& mesh) = delete;
	Mesh(Mesh&& mesh) noexcept;
	~Mesh();

	Mesh& operator=(const Mesh& mesh) = delete;
	Mesh& operator=(Mesh&& mesh) noexcept;

	GLsizei getVertexCount() const { return vertices.size(); }
	GLsizei getIndexCount() const { return indices.size(); }
	Vertex getVertexAt(uint32_t i) const { return vertices[i]; }
	uint32_t getIndexAt(uint32_t i) const { return indices[i]; }

	bool isInstanced() const { return modelMatrices != nullptr; }
	uint32_t getInstanceCount() const { return instanceCount; }
};