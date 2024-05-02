#include <iostream>
#include <fstream>
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

GLuint Shader::currentProgram = 0;

Shader::Shader(std::string_view vertex, std::string_view fragment, const VertexAttributes& attributes, LoadMethod method)
	: attributes(attributes), programID(0)
{
	if (method == LoadMethod::FromFile)
	{
		std::string vertexShader = readShaderFile(vertex);
		std::string fragmentShader = readShaderFile(fragment);

		compileShaders(vertexShader.c_str(), fragmentShader.c_str(), nullptr);
	}
	else
	{
		compileShaders(vertex.data(), fragment.data(), nullptr);
	}

	setInt("diffuseMap", 0);
	setInt("specularMap", 1);
}

Shader::Shader(std::string_view vertex, std::string_view fragment, std::string_view geometry, const VertexAttributes& attributes, LoadMethod method)
	: attributes(attributes), programID(0)
{
	if(method == LoadMethod::FromFile)
	{
		std::string vertexShader = readShaderFile(vertex);
		std::string fragmentShader = readShaderFile(fragment);
		std::string geometryShader = readShaderFile(geometry);

		compileShaders(vertexShader.c_str(), fragmentShader.c_str(), geometryShader.c_str());
	}
	else
	{
		compileShaders(vertex.data(), fragment.data(), geometry.data());
	}
}

Shader::Shader(Shader&& shader) noexcept : programID(shader.programID)
{
	shader.programID = 0;
}

Shader::~Shader()
{
	destroyShader();
}

Shader& Shader::operator=(Shader&& shader) noexcept
{
	this->programID = shader.programID;
	shader.programID = 0;
	return *this;
}

void Shader::destroyShader()
{
	if(programID == 0)
		return;

	glDeleteProgram(programID);
	programID = 0;
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

bool Shader::useShader() const
{
	if(currentProgram == programID)
		return false;

	glUseProgram(programID);
	currentProgram = programID;
	return true;
}

void Shader::compileShaders(const char* vertexCode, const char* fragmentCode, const char* geometryCode)
{
	programID = glCreateProgram();

	if(!programID)
	{
		std::cerr << "Failed to create shader!\n";
		return;
	}

	addShader(vertexCode, GL_VERTEX_SHADER);
	addShader(fragmentCode, GL_FRAGMENT_SHADER);

	if(geometryCode != nullptr)
		addShader(geometryCode, GL_GEOMETRY_SHADER);

	GLint result = 0;
	GLchar eLog[2048] = {0};

	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &result);

	if(!result)
	{
		glGetProgramInfoLog(programID, 2048 * sizeof(GLchar), NULL, eLog);
		std::cerr << "Error linking program: " << eLog << std::endl;
		destroyShader();
		return;
	}

	glValidateProgram(programID);
	glGetProgramiv(programID, GL_VALIDATE_STATUS, &result);

	if(!result)
	{
		glGetProgramInfoLog(programID, sizeof(eLog), NULL, eLog);
		std::cerr << "Error validating the program : " << eLog << std::endl;
		destroyShader();
		return;
	}
}

void Shader::addShader(const char* shaderCode, GLenum shaderType)
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
		std::cerr << "Error compiling the " << shaderType << " shader : " << eLog << '\n';
		glDeleteShader(theShader);
		destroyShader();
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