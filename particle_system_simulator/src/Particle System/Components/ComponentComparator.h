#pragma once

#include "Component.h"

class ComponentComparator
{
public:
	bool operator()(const Component* lhs, const Component* rhs) const
	{
		return lhs->getPriority() > rhs->getPriority();
	}
};