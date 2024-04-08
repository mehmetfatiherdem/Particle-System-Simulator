#pragma once

#include "RenderPipeline/Object/MeshRenderer.h"

bool compare(MeshRenderer* x, MeshRenderer* y)
{
	static auto refToPtr = []<typename T>(T& t) -> T* { return &t;};

	if((refToPtr(x->getShader())) != refToPtr(y->getShader()))
	{
		return refToPtr(x->getShader()) < refToPtr(y->getShader());
	}

	if(refToPtr(x->getMaterial()) != refToPtr(y->getMaterial()))
	{
		return refToPtr(x->getMaterial()) < refToPtr(y->getMaterial());
	}

	return false;
}