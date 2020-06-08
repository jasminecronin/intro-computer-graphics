// Quinn Masters & Jasmine Roebuck
// Defines a position, intensity, and ambient coefficient for a single light object

#include "Light.h"

Light::Light()
{
}

Light::~Light()
{
}

Light::Light(glm::vec3 p, float i, float a)
{
	position = p;
	intensity = i;
	ambient = a;
}


