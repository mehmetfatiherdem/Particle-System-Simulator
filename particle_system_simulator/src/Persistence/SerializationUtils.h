#pragma once

#include <string>
#include <glm/glm.hpp>

class Color3;
class Color4;
class Serializer;

template<typename T>
class BezierCurve;

namespace persistence::utils
{
	void serializeVector(Serializer& serializer, glm::vec2 vector, const std::string& objectName = "");
	void serializeVector(Serializer& serializer, glm::vec3 vector, const std::string& objectName = "");
	void serializeVector(Serializer& serializer, glm::vec4 vector, const std::string& objectName = "");
	void serializeColor(Serializer& serializer, const Color3& color, const std::string& objectName = "");
	void serializeColor(Serializer& serializer, const Color4& color, const std::string& objectName = "");
	void serializeBezierVector(Serializer& serializer, const BezierCurve<glm::vec3>& bezier, const std::string& objectName = "");
	void serializeBezierFloat(Serializer& serializer, const BezierCurve<float>& bezier, const std::string& objectName = "");
}
