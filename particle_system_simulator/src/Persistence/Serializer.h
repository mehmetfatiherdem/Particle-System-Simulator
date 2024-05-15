#pragma once

#include <string>
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
	void uint(unsigned int value) { writer.Uint(value); }
	void boolean(bool value) { writer.Bool(value); }
	void null() { writer.Null(); }

	std::string getString() const { return buffer.GetString(); }

	Writer& operator[](const std::string& key);
};
