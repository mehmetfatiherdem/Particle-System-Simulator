#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

struct Color4
{
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;

	Color4(const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular) : ambient(ambient),
		diffuse(diffuse), specular(specular) { }

	Color4(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular) : ambient(glm::vec4(ambient, 1.0f)),
		diffuse(glm::vec4(diffuse, 1.0f)), specular(glm::vec4(specular, 1.0f)) { }

	Color4(const glm::vec4& color) : ambient(color), diffuse(color), specular(color) { }

	Color4(const glm::vec3& color) : ambient(glm::vec4(color, 1.0f)), diffuse(glm::vec4(color, 1.0f)),
		specular(glm::vec4(color, 1.0f)) { }

	void setColor(const glm::vec4& color)
	{
		ambient = color;
		diffuse = color;
		specular = color;
	}

	void setColor(const glm::vec3& color)
	{
		ambient = glm::vec4(color, 1.0f);
		diffuse = glm::vec4(color, 1.0f);
		specular = glm::vec4(color, 1.0f);
	}
};

