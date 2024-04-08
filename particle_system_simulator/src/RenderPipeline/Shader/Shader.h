#pragma once

#include <string>
#include <string_view>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include "GeneralUtility/gl2fw3.h"
#include "Data/VertexAttributes.h"
#include "Data/LoadMethod.h"

class Shader
{
private:
	static GLuint currentProgram;

	GLuint programID;
	VertexAttributes vertexAttribs;

	void compileShaders(const char* vertexCode, const char* fragmentCode, const char* geometryCode = nullptr);
	void addShader(const char* shaderCode, GLenum shaderType);
	std::string readShaderFile(std::string_view file);
	void cleanup();

public:
	Shader() = delete;
	Shader(std::string_view vertex, std::string_view fragment, const VertexAttributes& vertexAttribs, LoadMethod method = LoadMethod::FromFile);
	Shader(std::string_view vertex, std::string_view fragment, std::string_view geometry, const VertexAttributes& vertexAttribs,
		LoadMethod method = LoadMethod::FromFile);

	Shader(Shader&& shader) noexcept;
	~Shader();

	Shader& operator=(Shader&& shader) noexcept;

	bool useShader() const;

	static Shader& genericShader();
	static Shader& instancedShader();
	static Shader& skyboxShader();
	static Shader& cursorShader();

	void setBool(std::string_view variable, bool value) const;
	void setInt(std::string_view variable, GLint value) const;
	void setFloat(std::string_view variable, GLfloat value) const;
	void setVector(std::string_view variable, const glm::vec2& value) const;
	void setVector(std::string_view variable, const glm::vec3& value) const;
	void setVector(std::string_view variable, const glm::vec4& value) const;
	void setMatrix4(std::string_view variable, const glm::mat4& value) const;

	VertexAttributes getVertexAttributes() const { return vertexAttribs; }
};
