#pragma once

#include "RenderPipeline/Transform/Transformable.h"
#include "RenderPipeline/Color/Color3.h"

class LightTracker;

class LightSource : public Transformable
{
protected:
	const LightTracker& lightTracker;
	Color3 color;

	LightSource() = delete;
	LightSource(const LightTracker& lightTracker, const TransformProps& transform, const Color3& color) : Transformable(transform),
		lightTracker(lightTracker), color(color) { }

	LightSource(const LightSource&) = default;
	LightSource(LightSource&&) = delete;
	~LightSource() = default;

	LightSource& operator=(const LightSource&) = delete;
	LightSource& operator=(LightSource&&) = delete;

	virtual void lightSourceUpdated() const = 0;

public:
	Color3 getColor() const { return color; }
	glm::vec3 getAmbientColor() const { return color.ambient; }
	glm::vec3 getDiffuseColor() const { return color.diffuse; }
	glm::vec3 getSpecularColor() const { return color.specular; }

	Transform& getTransform() = delete;
	const Transform& getTransform() const { return transform; }
	glm::mat4 getModelMatrix() const { return transform.getModelMatrix(); }

	void setColor(const Color3& color);
	void setColor(const glm::vec3& color);
	void setAmbientColor(const glm::vec3& ambient);
	void setDiffuseColor(const glm::vec3& diffuse);
	void setSpecularColor(const glm::vec3& specular);
};
