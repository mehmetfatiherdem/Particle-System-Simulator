#include <algorithm>
#include "RenderPipeline/Color/Color3.h"
#include "RenderPipeline/Color/Color4.h"
#include "GeneralUtility/BezierCurve.h"
#include "Persistence/Serializer.h"
#include "Persistence/Deserializer.h"
#include "SerializationUtils.h"

void persistence::utils::serializeVector(Serializer& serializer, glm::vec2 vector, const std::string& objectName)
{
	serializer.startArray(objectName);
	serializer.real(vector.x);
	serializer.real(vector.y);
	serializer.endArray();
}

void persistence::utils::serializeVector(Serializer& serializer, glm::vec3 vector, const std::string& objectName)
{
	serializer.startArray(objectName);
	serializer.real(vector.x);
	serializer.real(vector.y);
	serializer.real(vector.z);
	serializer.endArray();
}

void persistence::utils::serializeVector(Serializer& serializer, glm::vec4 vector, const std::string& objectName)
{
	serializer.startArray(objectName);
	serializer.real(vector.x);
	serializer.real(vector.y);
	serializer.real(vector.z);
	serializer.real(vector.w);
	serializer.endArray();
}

void persistence::utils::serializeColor(Serializer& serializer, const Color3& color, const std::string& objectName)
{
	serializer.startObject(objectName);
	serializeVector(serializer, color.ambient, "Ambient");
	serializeVector(serializer, color.diffuse, "Diffuse");
	serializeVector(serializer, color.specular, "Specular");
	serializer.endObject();
}

void persistence::utils::serializeColor(Serializer& serializer, const Color4& color, const std::string& objectName)
{
	serializer.startObject(objectName);
	serializeVector(serializer, color.ambient, "Ambient");
	serializeVector(serializer, color.diffuse, "Diffuse");
	serializeVector(serializer, color.specular, "Specular");
	serializer["Alpha"].real(color.alpha);
	serializer.endObject();
}

void persistence::utils::serializeBezier(Serializer& serializer, const BezierCurve<float>& bezier, const std::string& objectName)
{
	serializer.startObject(objectName);
	serializer["ControlPointsSize"].uint(bezier.size());
	serializer.startArray("ControlPoints");

	std::for_each(bezier.cbegin(), bezier.cend(), [&serializer](float controlPoint)
		{
			serializer.real(controlPoint);
		});

	serializer.endArray();
	serializer.endObject();
}

void persistence::utils::serializeBezier(Serializer& serializer, const BezierCurve<glm::vec3>& bezier, const std::string& objectName)
{
	serializer.startObject(objectName);
	serializer["ControlPointsSize"].uint(bezier.size());
	serializer.startArray("ControlPoints");

	std::for_each(bezier.cbegin(), bezier.cend(), [&serializer](const glm::vec3& controlPoint)
		{
			serializeVector(serializer, controlPoint);
		});

	serializer.endArray();
	serializer.endObject();
}

void persistence::utils::deserializeVector(const Deserializer& deserializer, glm::vec2* vector)
{
	vector->x = deserializer[0].getReal();
	vector->y = deserializer[1].getReal();
}

void persistence::utils::deserializeVector(const Deserializer& deserializer, glm::vec3* vector)
{
	vector->x = deserializer[0].getReal();
	vector->y = deserializer[1].getReal();
	vector->z = deserializer[2].getReal();
}

void persistence::utils::deserializeVector(const Deserializer& deserializer, glm::vec4* vector)
{
	vector->x = deserializer[0].getReal();
	vector->y = deserializer[1].getReal();
	vector->z = deserializer[2].getReal();
	vector->w = deserializer[3].getReal();
}

void persistence::utils::deserializeColor(const Deserializer& deserializer, Color3* color)
{
	deserializeVector(deserializer["Ambient"], &color->ambient);
	deserializeVector(deserializer["Diffuse"], &color->diffuse);
	deserializeVector(deserializer["Specular"], &color->specular);
}

void persistence::utils::deserializeColor(const Deserializer& deserializer, Color4* color)
{
	deserializeVector(deserializer["Ambient"], &color->ambient);
	deserializeVector(deserializer["Diffuse"], &color->diffuse);
	deserializeVector(deserializer["Specular"], &color->specular);
	color->alpha = deserializer["Alpha"].getReal();
}

void persistence::utils::deserializeBezier(const Deserializer& deserializer, BezierCurve<float>* bezier)
{
	size_t size = deserializer["ControlPointsSize"].getUint();
	const Deserializer& array = deserializer["ControlPoints"];

	bezier->clear();

	for (size_t i = 0; i < size; ++i)
	{
		bezier->pushControlPoint(array[i].getReal());
	}
}

void persistence::utils::deserializeBezier(const Deserializer& deserializer, BezierCurve<glm::vec3>* bezier)
{
	size_t size = deserializer["ControlPointsSize"].getUint();
	const Deserializer& array = deserializer["ControlPoints"];
	glm::vec3 vec;

	bezier->clear();

	for (size_t i = 0; i < size; ++i)
	{
		deserializeVector(array[i], &vec);
		bezier->pushControlPoint(vec);
	}
}