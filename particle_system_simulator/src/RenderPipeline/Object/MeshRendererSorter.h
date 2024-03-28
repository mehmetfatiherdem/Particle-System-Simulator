#pragma once

#include "MeshRenderer.h"

bool compare(const MeshRenderer& x, const MeshRenderer& y)
{
	if(x.getShader() != y.getShader())
	{
		return x.getShader() < y.getShader();
	}

	if(x.getMaterial() != y.getMaterial())
	{
		return x.getMaterial() < y.getMaterial();
	}

	return false;
}