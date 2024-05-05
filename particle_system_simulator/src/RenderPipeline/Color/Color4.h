#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

struct Color4
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float alpha;

	Color4() : ambient(glm::vec3{0.0f}), diffuse(glm::vec3{0.0f}), specular(glm::vec3{0.0f}), alpha(0.0f) { }

	Color4(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float alpha) : ambient(ambient),
		diffuse(diffuse), specular(specular), alpha(alpha) { }

	Color4(const glm::vec4& color) : ambient(color), diffuse(color), specular(color), alpha(color.a) { }

	Color4(const glm::vec3& color, float alpha) : ambient(color), diffuse(color), specular(color), alpha(alpha) { }

	Color4(const glm::vec3& color) : ambient(color), diffuse(color), specular(color), alpha(1.0f) {}

	bool operator==(const Color4& other) const
	{
		return ambient == other.ambient &&
			diffuse == other.diffuse &&
			specular == other.specular &&
			alpha == other.alpha;
	}

	void setColor(const glm::vec4& color)
	{
		ambient = color;
		diffuse = color;
		specular = color;
		alpha = color.a;
	}

	void setColor(const glm::vec3& color, float alpha)
	{
		ambient = color;
		diffuse = color;
		specular = color;
		this->alpha = alpha;
	}

	void setColor(const glm::vec3& color)
	{
		ambient = color;
		diffuse = color;
		specular = color;
		alpha = 1.0f;
	}
};

