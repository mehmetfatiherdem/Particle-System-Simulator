#include <glm/ext/quaternion_float.hpp>
#include "Transform.h"

glm::mat4 Transform::getModelMatrix() const
{
	glm::mat4 translate = glm::translate(glm::mat4(1.0), position);
	glm::mat4 rotate = glm::mat4_cast(rotation);
	glm::mat4 scale = glm::scale(glm::mat4(1.0), _scale);

	return translate * rotate * scale;
}

void Transform::translate(const glm::vec3& displacement)
{
	this->position += displacement;
}

void Transform::rotate(const glm::vec3& rotation)
{
	glm::quat rot = glm::quat(rotation);
	this->rotation = rot * this->rotation;
}

void Transform::rotate(const glm::quat& rotation)
{
	this->rotation = rotation * this->rotation;
}

void Transform::rotateAround(const glm::vec3& axis, float angle)
{
	glm::quat rot = glm::angleAxis(angle, glm::normalize(axis));
	rotation = rot * rotation;
}

void Transform::rotateAround(const glm::vec3& point, const glm::vec3& axis, float angle)
{
	glm::mat4 translateToOrigin = glm::translate(glm::mat4(1.0f), -point);
	glm::quat rot = glm::angleAxis(angle, glm::normalize(axis));
	rotation = rot * rotation;
	glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), point);
	position = glm::vec3(translateBack * translateToOrigin * glm::vec4(position, 1.0f));
}

void Transform::rotateAround(const Transform& transform, const glm::vec3& axis, float angle)
{
	rotateAround(transform.getPosition(), glm::normalize(axis), angle);
}

void Transform::scale(const glm::vec3& scale)
{
	this->_scale *= scale;
}

void Transform::lookAt(const glm::vec3& point)
{
	rotation = glm::quatLookAt(glm::normalize(point - this->position), WORLD_UP);
}

void Transform::lookAt(const Transform& transform)
{
	glm::vec3 targetPosition = transform.getPosition();
	lookAt(targetPosition);
}

glm::vec3 Transform::getRightVector() const
{
	glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
	return rotationMatrix[0];
}

glm::vec3 Transform::getUpVector() const
{
	glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
	return rotationMatrix[1];
}

glm::vec3 Transform::getForwardVector() const
{
	glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
	return rotationMatrix[2];
}

void Transform::setRightVector(const glm::vec3& right)
{
	glm::vec3 currentRight = getRightVector();
	glm::vec3 axis = glm::cross(currentRight, right);
	float angle = glm::acos(glm::dot(currentRight, right));
	rotateAround(axis, angle);
}

void Transform::setUpVector(const glm::vec3& up)
{
	glm::vec3 currentUp = getUpVector();
	glm::vec3 axis = glm::cross(currentUp, up);
	float angle = glm::acos(glm::dot(currentUp, up));
	rotateAround(axis, angle);
}

void Transform::setForwardVector(const glm::vec3& forward)
{
	glm::vec3 currentForward = getForwardVector();
	glm::vec3 axis = glm::cross(currentForward, forward);
	float angle = glm::acos(glm::dot(currentForward, forward));
	rotateAround(axis, angle);
}