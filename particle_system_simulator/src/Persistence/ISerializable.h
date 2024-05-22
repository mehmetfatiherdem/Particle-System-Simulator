#pragma once

#include <string>

class Serializer;

class ISerializable
{
public:
	virtual void serialize(Serializer& serializer, const std::string& objectName = "") const = 0;
};