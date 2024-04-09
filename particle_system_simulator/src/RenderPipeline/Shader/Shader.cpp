#include <iostream>
#include <fstream>
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include "GeneralUtility/strtok.h"
#include "Shader.h"

GLuint Shader::currentProgram = 0;

Shader::Shader(std::string_view vertex, std::string_view fragment, const VertexAttributes& vertexAttribs,
	LoadMethod method) : Shader(vertex, fragment, "", vertexAttribs, method) { }

Shader::Shader(std::string_view vertex, std::string_view fragment, std::string_view geometry, const VertexAttributes& vertexAttribs,
	LoadMethod method) : vertexAttribs(vertexAttribs)
{
	auto tokens = tokenize(vertex, '/');
	std::string vertexFile = tokens[tokens.size() - 1];
	tokens = tokenize(fragment, '/');
	std::string fragmentFile = tokens[tokens.size() - 1];
	tokens = tokenize(geometry, '/');
	std::string geometryFile = tokens.size() == 0 ? "" : tokens[tokens.size() - 1];

	if(method == LoadMethod::FromFile)
	{
		compileShaders(vertexFile, readShaderFile(vertex).c_str(), fragmentFile, readShaderFile(fragment).c_str(), geometryFile,
			geometry.empty() ? nullptr : readShaderFile(geometry).c_str());
	}
	else
	{
		compileShaders(vertexFile, vertex.data(), fragmentFile, fragment.data(), geometryFile, geometry.empty() ? nullptr : geometry.data());
	}
}

Shader::Shader(Shader&& shader) noexcept : programID(shader.programID), vertexAttribs(shader.vertexAttribs)
{
	shader.programID = 0;
}

Shader::~Shader()
{
	cleanup();
}

Shader& Shader::operator=(Shader&& shader) noexcept
{
	this->programID = shader.programID;
	shader.programID = 0;
	return *this;
}

void Shader::cleanup()
{
	if(programID == 0)
		return;

	glDeleteProgram(programID);
	programID = 0;
}

bool Shader::useShader() const
{
	if(currentProgram == programID)
		return false;

	glUseProgram(programID);
	currentProgram = programID;
	return true;
}

void Shader::compileShaders(std::string_view vertexFile, const char* vertexCode, std::string_view fragmentFile,
	const char* fragmentCode, std::string_view geometryFile, const char* geometryCode)
{
	programID = glCreateProgram();

	if(!programID)
	{
		std::cerr << "Failed to create shader!\n";
		return;
	}

	addShader(vertexFile, vertexCode, GL_VERTEX_SHADER);
	addShader(fragmentFile, fragmentCode, GL_FRAGMENT_SHADER);

	if(geometryCode != nullptr)
	{
		addShader(geometryFile, geometryCode, GL_GEOMETRY_SHADER);
	}

	GLint result = 0;
	GLchar eLog[2048] = {0};

	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &result);

	if(!result)
	{
		glGetProgramInfoLog(programID, 2048 * sizeof(GLchar), NULL, eLog);
		std::cerr << "Error linking programs: " << vertexFile << ", " << fragmentFile << (geometryCode == nullptr ? "" : geometryFile) << ":\n" << eLog << std::endl;
		cleanup();
		return;
	}

	glValidateProgram(programID);
	glGetProgramiv(programID, GL_VALIDATE_STATUS, &result);

	if(!result)
	{
		glGetProgramInfoLog(programID, sizeof(eLog), NULL, eLog);
		std::cerr << "Error validating the program : " << eLog << std::endl;
		cleanup();
		return;
	}
}

void Shader::addShader(std::string_view shaderFile, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[2048] = {0};

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if(!result)
	{
		glGetShaderInfoLog(theShader, 2048, NULL, eLog);
		std::cerr << "Error compiling the " << shaderFile << " shader : " << eLog << '\n';
		glDeleteShader(theShader);
		cleanup();
		return;
	}

	glAttachShader(programID, theShader);
	glDeleteShader(theShader);
}

std::string Shader::readShaderFile(std::string_view address)
{
	std::ifstream fs(address.data(), std::ios::in);

	if(!fs.is_open())
	{
		std::cerr << "Error opening the file : " << address << '\n';
		return "";
	}

	std::string content = "";
	std::string line = "";

	while(!fs.eof())
	{
		std::getline(fs, line);
		line.append(1, '\n');
		content.append(line);
	}

	fs.close();
	return content;
}

Shader& Shader::genericShader()
{
	static Shader shader{"Resources/Shaders/Generic/generic.vert", "Resources/Shaders/Generic/generic.frag", VertexAttributes::generic()};
	return shader;
}

Shader& Shader::instancedShader()
{
	static Shader shader{"Resources/Shaders/Instanced/instanced.vert", "Resources/Shaders/Generic/generic.frag", VertexAttributes::instanced()};
	return shader;
}

Shader& Shader::skyboxShader()
{
	static Shader shader{"Resources/Shaders/Skybox/skybox.vert", "Resources/Shaders/Skybox/skybox.frag", VertexAttributes::skybox()};
	return shader;
}

Shader& Shader::cursorShader()
{
	static Shader shader{"Resources/Shaders/Cursor/cursor.vert", "Resources/Shaders/Cursor/cursor.frag", VertexAttributes::cursor()};
	return shader;
}

void Shader::setBool(std::string_view variable, bool value) const
{
	glUniform1i(glGetUniformLocation(programID, variable.data()), (int)value);
}

void Shader::setInt(std::string_view variable, GLint value) const
{
	glUniform1i(glGetUniformLocation(programID, variable.data()), value);
}

void Shader::setFloat(std::string_view variable, GLfloat value) const
{
	glUniform1f(glGetUniformLocation(programID, variable.data()), value);
}

void Shader::setVector(std::string_view variable, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(programID, variable.data()), 1, glm::value_ptr(value));
}

void Shader::setVector(std::string_view variable, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(programID, variable.data()), 1, glm::value_ptr(value));
}

void Shader::setVector(std::string_view variable, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(programID, variable.data()), 1, glm::value_ptr(value));
}

void Shader::setMatrix4(std::string_view variable, const glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(programID, variable.data()), 1, GL_FALSE, glm::value_ptr(value));
}