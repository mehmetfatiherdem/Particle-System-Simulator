#pragma once

#include "Entity.h"

class IComponent
{
public:
	virtual ~IComponent() = default;
	virtual Entity* operator->() = 0;
};

