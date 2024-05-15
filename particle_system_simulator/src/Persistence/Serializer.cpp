#include "Serializer.h"

void Serializer::startObject(const std::string& objectName)
{
	if (!objectName.empty())
	{
		writer.Key(objectName.c_str());
	}

	writer.StartObject();
}

void Serializer::startArray(const std::string& arrayName)
{
	if (!arrayName.empty())
	{
		writer.Key(arrayName.c_str());
	}

	writer.StartArray();
}

Writer& Serializer::operator[](const std::string& key)
{
	writer.Key(key.c_str());
	return writer;
}