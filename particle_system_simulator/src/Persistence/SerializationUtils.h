#pragma once

#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Deserializer;
class Serializer;
class Color3;
class Color4;

template <typename T>
class BezierCurve;

namespace persistence::utils
{
	void serializeVector(Serializer& serializer, glm::vec2 vector, const std::string& objectName = "");
	void serializeVector(Serializer& serializer, glm::vec3 vector, const std::string& objectName = "");
	void serializeVector(Serializer& serializer, glm::vec4 vector, const std::string& objectName = "");
	void serializeColor(Serializer& serializer, const Color3& color, const std::string& objectName = "");
	void serializeColor(Serializer& serializer, const Color4& color, const std::string& objectName = "");
	void serializeBezier(Serializer& serializer, const BezierCurve<float>& bezier, const std::string& objectName = "");
	void serializeBezier(Serializer& serializer, const BezierCurve<glm::vec3>& bezier, const std::string& objectName = "");

	void deserializeVector(const Deserializer& deserializer, glm::vec2* vector);
	void deserializeVector(const Deserializer& deserializer, glm::vec3* vector);
	void deserializeVector(const Deserializer& deserializer, glm::vec4* vector);
	void deserializeColor(const Deserializer& deserializer, Color3* color);
	void deserializeColor(const Deserializer& deserializer, Color4* color);
	void deserializeBezier(const Deserializer& deserializer, BezierCurve<float>* bezier);
	void deserializeBezier(const Deserializer& deserializer, BezierCurve<glm::vec3>* bezier);
}
