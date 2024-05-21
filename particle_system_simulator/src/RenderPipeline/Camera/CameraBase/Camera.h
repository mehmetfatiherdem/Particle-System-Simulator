#pragma once

#include <stdint.h>
#include "RenderPipeline/Camera/Data/CameraType.h"
#include "RenderPipeline/Transform/Transformable.h"
#include "Persistence/ISerializable.h"

class Deserializer;

class Camera : public Transformable, public ISerializable
{
protected:
	CameraType cameraType;
	uint32_t width;
	uint32_t height;
	float aspectRatio;
	float fov;
	float near;
	float far;
	float left;
	float right;
	float top;
	float bottom;

public:
	Camera() = delete;
	Camera(const TransformProps& transform, uint32_t width, uint32_t height, CameraType cameraType = CameraType::Perspective,
		float fov = glm::radians(45.0f), float near = 0.1f, float far = 1000.0f, float left = -8.0f, float right = 8.0f,
		float top = 4.5f, float bottom = -4.5f);

	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix(float useFov = -1.0f) const;
	glm::mat4 getViewProjectionMatrix(float useFov = -1.0f) const;

	uint32_t getWidth() const { return width; }
	uint32_t getHeight() const { return height; }
	float getAspectRatio() const { return aspectRatio; }
	float getFov() const { return fov; }
	float getNear() const { return near; }
	float getFar() const { return far; }
	float getLeft() const { return left; }
	float getRight() const { return right; }
	float getTop() const { return top; }
	float getBottom() const { return bottom; }
	CameraType getCameraType() const { return cameraType; }

	void setWidth(uint32_t width);
	void setHeight(uint32_t height);
	void setAspectRatio(uint32_t width, uint32_t height);
	void setFov(float fov) { this->fov = fov; }
	void setNear(float near) { this->near = near; }
	void setFar(float far) { this->far = far; }
	void setLeft(float left) { this->left = left; }
	void setRight(float right) { this->right = right; }
	void setTop(float top) { this->top = top; }
	void setBottom(float bottom) { this->bottom = bottom; }
	void setCameraType(CameraType cameraType) { this->cameraType = cameraType; }

	virtual void serialize(Serializer& serializer, const std::string& objectName = "Camera") const override;
	void deserialize(const Deserializer& deserializer);
};