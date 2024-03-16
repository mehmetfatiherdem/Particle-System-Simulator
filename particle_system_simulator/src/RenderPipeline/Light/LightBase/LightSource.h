#pragma once

#include "RenderPipeline/Transform/Transform.h"
#include "RenderPipeline/Color/Color3.h"

class LightTracker;

class LightSource
{
protected:
	const LightTracker& lightTracker;
	Transform transform;
	Color3 color;

	LightSource() = delete;
	LightSource(const LightTracker& lightTracker, const Color3& color);
	LightSource(const LightTracker& lightTracker, const glm::vec3& position, const Color3& color);

	virtual void lightSourceUpdated() const = 0;

public:
	Color3 getColor() const { return color; }
	glm::vec3 getAmbientColor() const { return color.ambient; }
	glm::vec3 getDiffuseColor() const { return color.diffuse; }
	glm::vec3 getSpecularColor() const { return color.specular; }

	const Transform& getTransform() const { return transform; }
	glm::mat4 getModelMatrix() const { return transform.getModelMatrix(); }

	virtual void translate(const glm::vec3& displacement);
	virtual void rotate(const glm::vec3& rotation);
	virtual void rotate(const glm::quat& rotation);
	virtual void rotateAround(const glm::vec3& axis, float angle);
	virtual void rotateAround(const glm::vec3& point, const glm::vec3& axis, float angle);
	virtual void rotateAround(const Transform& transform, const glm::vec3& axis, float angle);
	void scale(const glm::vec3& scale) { this->transform.scale(scale); }
	virtual void lookAt(const glm::vec3& position);
	virtual void lookAt(const Transform& transform);
	virtual void setRightVector(const glm::vec3& right);
	virtual void setUpVector(const glm::vec3& up);
	virtual void setForwardvector(const glm::vec3& forward);
	virtual void setPosition(const glm::vec3& position);
	virtual void setRotation(const glm::quat& rotation);
	virtual void setEulerRotation(const glm::vec3& rotation);
	void setScale(const glm::vec3& scale) { this->transform.setScale(scale); }

	void setColor(const Color3& color);
	void setColor(const glm::vec3& color);
	void setAmbientColor(const glm::vec3& ambient);
	void setDiffuseColor(const glm::vec3& diffuse);
	void setSpecularColor(const glm::vec3& specular);
};
