#pragma once

#include <stdint.h>

struct VertexAttributes
{
	bool hasPosition;
	bool hasNormal;
	bool hasUV;
	bool isInstanced;
	uint32_t approximateCount;

	VertexAttributes() : hasPosition(true), hasNormal(true), hasUV(true), isInstanced(false), approximateCount(0) { }
	VertexAttributes(bool hasPosition, bool hasNormal, bool hasUV, bool isInstanced, uint32_t approximateCount = 0) :
		hasPosition(hasPosition), hasNormal(hasNormal), hasUV(hasUV), isInstanced(isInstanced),
		approximateCount(isInstanced ? approximateCount : 0) { }

	static VertexAttributes generic() { return VertexAttributes(); }
	static VertexAttributes instanced() { return VertexAttributes(true, true, true, true, 125); }
	static VertexAttributes skybox() { return VertexAttributes(true, false, false, false); }
	static VertexAttributes cursor() { return VertexAttributes(true, false, true, false); }
};