#include <algorithm>
#include "RenderPipeline/Color/Color4.h"
#include "RenderPipeline/Color/Color3.h"
#include "GeneralUtility/BezierCurve.h"
#include "Serializer.h"
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
	serializer["Alpha"].Double(color.alpha);
	serializer.endObject();
}

void persistence::utils::serializeBezierFloat(Serializer& serializer, const BezierCurve<float>& bezier, const std::string& objectName)
{
	serializer.startObject(objectName);
	serializer["ControlPointsSize"].Uint(bezier.size());
	serializer.startArray("ControlPoints");

	std::for_each(bezier.cbegin(), bezier.cend(), [&serializer](float controlPoint)
		{
			serializer.real(controlPoint);
		});

	serializer.endArray();
	serializer.endObject();
}

void persistence::utils::serializeBezierVector(Serializer& serializer, const BezierCurve<glm::vec3>& bezier, const std::string& objectName)
{
	serializer.startObject(objectName);
	serializer["ControlPointsSize"].Uint(bezier.size());
	serializer.startArray("ControlPoints");

	std::for_each(bezier.cbegin(), bezier.cend(), [&serializer](const glm::vec3& controlPoint)
		{
			serializeVector(serializer, controlPoint);
		});

	serializer.endArray();
	serializer.endObject();
}