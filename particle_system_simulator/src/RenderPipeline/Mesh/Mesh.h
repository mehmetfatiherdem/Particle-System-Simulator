#pragma once

#include <vector>
#include <memory>
#include <optional>
#include <glm/mat4x4.hpp>
#include <stdint.h>
#include "GeneralUtility/gl2fw3.h"
#include "RenderPipeline/Shader/Data/VertexAttributes.h"
#include "Data/Vertex.h"

template<class T>
using SharedVector = std::shared_ptr<std::vector<T>>;

class Mesh
{
private:
	friend class MeshRenderer;
	friend class Skybox;
	friend class Cursor;

	GLuint VAO;
	GLuint buffers[3];
	std::optional<VertexAttributes> props;

	SharedVector<Vertex> vertices;
	SharedVector<uint32_t> indices;
	SharedVector<glm::mat4> instanceMatrices;

	uint32_t updatedSinceLastDraw;

	void initialize();
	void initialize(VertexAttributes vertexAttribs);
	void draw() const;
	void setModelMatrix(const glm::mat4& model);
	void addInstance();
	void removeInstance();
	void cleanup();

	bool isInstanced() const { return props.has_value() && props->isInstanced; }
	uint32_t getInstanceCount() const { return instanceMatrices->size(); }

public:
	Mesh() = delete;
	Mesh(std::vector<Vertex>&& vertices, std::vector<uint32_t>&& indices);
	Mesh(const Mesh& mesh);
	Mesh(Mesh&& mesh) noexcept;
	~Mesh();

	Mesh& operator=(const Mesh& mesh);
	Mesh& operator=(Mesh&& mesh) noexcept;

	GLsizei getVertexCount() const { return vertices->size(); }
	GLsizei getIndexCount() const { return indices->size(); }
	Vertex getVertexAt(uint32_t i) const { return (*vertices)[i]; }
	uint32_t getIndexAt(uint32_t i) const { return (*indices)[i]; }
};
