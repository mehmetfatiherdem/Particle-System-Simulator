#pragma once

#include <vector>
#include <memory>
#include <optional>
#include <glm/mat4x4.hpp>
#include "GeneralUtility/gl2fw3.h"
#include "GeneralUtility/Aliases.h"
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
	VertexAttributes props;
	std::optional<VertexAttributes> props;

	SharedVector<Vertex> vertices;
	SharedVector<uint> indices;
	SharedVector<glm::mat4> instanceMatrices;

	uint updatedSinceLastDraw;

	void initializeMesh(const VertexAttributes& vertexAttribs);

	void draw() const;
	void setModelMatrix(const glm::mat4& model);
	void addInstance();
	void removeInstance();

	void cleanup();

public:
	Mesh() = delete;
	Mesh(std::vector<Vertex>&& vertices, std::vector<uint>&& indices);
	Mesh(const Mesh& mesh);
	Mesh(Mesh&& mesh) noexcept;
	~Mesh();

	Mesh& operator=(const Mesh& mesh);
	Mesh& operator=(Mesh&& mesh) noexcept;

	GLsizei getVertexCount() const { return vertices->size(); }
	GLsizei getIndexCount() const { return indices->size(); }
	Vertex getVertexAt(uint i) const { return (*vertices)[i]; }
	uint getIndexAt(uint i) const { return (*indices)[i]; }

	bool isInstanced() const { return props.isInstanced; }
	uint getInstanceCount() const { return instanceMatrices->size(); }
};
