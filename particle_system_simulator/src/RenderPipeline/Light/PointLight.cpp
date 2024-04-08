#include "LightManagement/LightTracker.h"
#include "PointLight.h"

void PointLight::lightSourceUpdated() const
{
	lightTracker.updateLight(this);
}
