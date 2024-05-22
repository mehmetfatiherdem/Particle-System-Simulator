#pragma once

#include <string>
#include <stdint.h>
#include "rapidjson/prettywriter.h"

using Writer = rapidjson::PrettyWriter<rapidjson::StringBuffer>;

class Serializer
{
private:
	rapidjson::StringBuffer buffer;
	Writer writer;

public:
	Serializer() : buffer(), writer(buffer) {}

	void startObject(const std::string& objectName = "");
	void startArray(const std::string& arrayName = "");

	void endObject() { writer.EndObject(); }
	void endArray() { writer.EndArray(); }

	void key(const char* key) { writer.Key(key); }

	void string(const std::string& value) { writer.String(value.c_str()); }
	void real(double value) { writer.Double(value); }
	void uint(uint32_t value) { writer.Uint(value); }
	void integer(int value) { writer.Int(value); }
	void boolean(bool value) { writer.Bool(value); }
	void null() { writer.Null(); }

	std::string getJsonString() const { return buffer.GetString(); }

	Serializer& operator[](const std::string& key);
};
