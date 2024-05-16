#pragma once

#include <string>
#include <stdint.h>
#include <rapidjson/document.h>

using Document = rapidjson::Document;
using size_type = rapidjson::SizeType;

class Deserializer
{
private:
	rapidjson::Value& value;

public:
	Deserializer(Document& doc) : value(doc) { }
	Deserializer(rapidjson::Value& value) : value(value) { }

	const Deserializer operator[](const std::string& key) const { return value[key.c_str()]; }
	const Deserializer operator[](size_type index) const { return value[index]; }

	static Document createDocument(const std::string& json);

	bool isNull() const { return value.IsNull(); }
	bool isObject() const { return value.IsObject(); }
	bool isArray() const { return value.IsArray(); }

	std::string getString() const { return value.GetString(); }
	double getReal() const { return value.GetDouble(); }
	uint32_t getUint() const { return value.GetUint(); }
	int getInt() const { return value.GetInt(); }
	bool getBoolean() const { return value.GetBool(); }

	rapidjson::GenericArray<false, rapidjson::Value> getArray() const { return value.GetArray(); }
	size_type size() const { return value.Size(); }
};