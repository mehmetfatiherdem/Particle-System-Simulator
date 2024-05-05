#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform
{
private:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 _scale;

	static bool isValidAngle(float angle);
	static bool isValidAxis(const glm::vec3& axis);

public:
	static constexpr glm::vec3 WORLD_UP{0.0f, 1.0f, 0.0f};

	Transform();
	Transform(const glm::vec3& position);
	Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale = glm::vec3{1.0f, 1.0f, 1.0f});
	Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale = glm::vec3{1.0f, 1.0f, 1.0f});

	glm::mat4 getModelMatrix() const;

	void translate(const glm::vec3& displacement);
	void rotate(const glm::vec3& rotation);
	void rotate(const glm::quat& rotation);
	void rotateAround(const glm::vec3& axis, float angle);
	void rotateAround(const glm::vec3& point, const glm::vec3& axis, float angle);
	void rotateAround(const Transform& transform, const glm::vec3& axis, float angle);
	void scale(const glm::vec3& scale);

	void lookAt(const Transform& transform);
	void lookAt(const glm::vec3& position);

	glm::vec3 getRightVector() const;
	glm::vec3 getUpVector() const;
	glm::vec3 getForwardVector() const;
	
	const glm::vec3 getPosition() const { return position; }
	const glm::quat getRotation() const { return rotation; }
	glm::vec3 getEulerRotation() const { return glm::eulerAngles(rotation); }
	const glm::vec3 getScale() const { return _scale; }

	void setRightVector(const glm::vec3& right);
	void setUpVector(const glm::vec3& up);
	void setForwardVector(const glm::vec3& forward);

	void setPosition(const glm::vec3& position) { this->position = position; }
	void setRotation(const glm::quat& rotation) { this->rotation = rotation; }
	void setEulerRotation(const glm::vec3& rotation) { this->rotation = glm::quat{rotation}; }
	void setScale(const glm::vec3& scale) { this->_scale = scale; }
};

