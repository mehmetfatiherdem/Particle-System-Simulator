#pragma once

#include <glm/vec3.hpp>

struct Color3
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	Color3(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular) : ambient(ambient), diffuse(diffuse),
		specular(specular) { }

	Color3(const glm::vec3& color) : ambient(color), diffuse(color), specular(color) { }

	void setColor(const glm::vec3& color)
	{
		ambient = color;
		diffuse = color;
		specular = color;
	}
};

