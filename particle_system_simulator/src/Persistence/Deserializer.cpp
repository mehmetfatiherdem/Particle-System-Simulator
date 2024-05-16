#include "Deserializer.h"

Document Deserializer::createDocument(const std::string& json)
{
	Document doc;
	doc.Parse(json.c_str());
	return doc;
}