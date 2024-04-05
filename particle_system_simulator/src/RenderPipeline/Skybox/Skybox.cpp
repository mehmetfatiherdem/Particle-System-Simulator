#include <iostream>
#include <vector>
#include "GeneralUtility/stb_wrapper/stb_wrapper.h"
#include "Skybox.h"

void addSurface(GLenum targetSide, std::string_view file)
{
	int width, height, noOfChannels;
	unsigned char* data = w_stbi_load(file.data(), &width, &height, &noOfChannels, 0);

	if(data)
	{
		glTexImage2D(targetSide, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		w_stbi_image_free(data);
	}
	else
	{
		std::cerr << "Cubemap texture failed to load at path: " << file << "\n";
	}
}

Skybox::Skybox(std::string_view right, std::string_view left, std::string_view top, std::string_view bottom, std::string_view front, std::string_view back) : shader("Resources/Shaders/skybox.vert", "Resources/Shaders/skybox.frag")
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	addSurface(GL_TEXTURE_CUBE_MAP_POSITIVE_X, right);
	addSurface(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left);
	addSurface(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top);
	addSurface(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom);
	addSurface(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, front);
	addSurface(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, back);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	std::vector<glm::vec3> vertices{
		glm::vec3{+0.5f, +0.5f, +0.5f},
		glm::vec3{-0.5f, +0.5f, +0.5f},
		glm::vec3{-0.5f, -0.5f, +0.5f},
		glm::vec3{+0.5f, -0.5f, +0.5f},
		glm::vec3{+0.5f, +0.5f, -0.5f},
		glm::vec3{+0.5f, +0.5f, +0.5f},
		glm::vec3{+0.5f, -0.5f, +0.5f},
		glm::vec3{+0.5f, -0.5f, -0.5f},
		glm::vec3{-0.5f, +0.5f, -0.5f},
		glm::vec3{+0.5f, +0.5f, -0.5f},
		glm::vec3{+0.5f, -0.5f, -0.5f},
		glm::vec3{-0.5f, -0.5f, -0.5f},
		glm::vec3{-0.5f, +0.5f, +0.5f},
		glm::vec3{-0.5f, +0.5f, -0.5f},
		glm::vec3{-0.5f, -0.5f, -0.5f},
		glm::vec3{-0.5f, -0.5f, +0.5f},
		glm::vec3{+0.5f, +0.5f, -0.5f},
		glm::vec3{-0.5f, +0.5f, -0.5f},
		glm::vec3{-0.5f, +0.5f, +0.5f},
		glm::vec3{+0.5f, +0.5f, +0.5f},
		glm::vec3{+0.5f, -0.5f, +0.5f},
		glm::vec3{-0.5f, -0.5f, +0.5f},
		glm::vec3{-0.5f, -0.5f, -0.5f},
		glm::vec3{+0.5f, -0.5f, -0.5f},
	};

	std::vector<unsigned int> indices{
		 0,  1,  2,  2,  3,  0,
		 4,  5,  6,  6,  7,  4,
		 8,  9, 10, 10, 11,  8,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20,
	};

	this->indicesSize = indices.size();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint temp[2];

	glGenBuffers(2, temp);
	VBO = temp[0];
	IBO = temp[1];

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Skybox::~Skybox()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
}

void Skybox::render(const glm::mat4& view, const glm::mat4& projection)
{
	glDepthMask(GL_TRUE);
	glCullFace(GL_FRONT);

	shader.useShader();
	shader.setMatrix4("view_projection", projection * glm::mat4(glm::mat3(view)));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glCullFace(GL_BACK);
	glDepthMask(GL_TRUE);
}