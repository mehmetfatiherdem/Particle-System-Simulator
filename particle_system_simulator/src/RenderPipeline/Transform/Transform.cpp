#include <glm/ext/quaternion_float.hpp>
#include "GeneralUtility/MathConstants.h"
#include "Transform.h"

Transform::Transform() : position(0.0f, 0.0f, 0.0f), rotation(1.0f, 0.0f, 0.0f, 0.0f), _scale(1.0f, 1.0f, 1.0f) { }

Transform::Transform(const glm::vec3& position) : position(position), rotation(1.0f, 0.0f, 0.0f, 0.0f), _scale(1.0f, 1.0f, 1.0f) { }

Transform::Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale) : position(position), rotation(rotation), _scale(scale) { }

Transform::Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) : position(position), rotation(glm::quat{rotation}), _scale(scale) { }

bool Transform::isValidAngle(float angle)
{
	if (!std::isnormal(angle)) return false;
	return std::abs(angle) > EPSILON;
}

bool Transform::isValidAxis(const glm::vec3& axis)
{
	if (isnan(axis.x) || isinf(axis.x)) return false;
	if (isnan(axis.y) || isinf(axis.y)) return false;
	if (isnan(axis.z) || isinf(axis.z)) return false;

	return glm::abs(1.0f - glm::length(axis)) <= EPSILON;
}

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
	if (!isValidAngle(angle) || !isValidAxis(axis)) return;
	glm::quat rot = glm::angleAxis(angle, glm::normalize(axis));
	rotation = rot * rotation;
}

void Transform::rotateAround(const glm::vec3& point, const glm::vec3& axis, float angle)
{
	if (!isValidAngle(angle) || !isValidAxis(axis)) return;
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

void Transform::lookAt(const glm::vec3& position)
{
	glm::vec3 diff = this->position - position;
	if (glm::length(diff) < EPSILON) return;
	rotation = glm::quatLookAt(glm::normalize(diff), WORLD_UP);
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