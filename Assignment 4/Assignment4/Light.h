// Quinn Masters & Jasmine Roebuck
// Defines a position, intensity, and ambient coefficient for a single light object

#pragma once
#include <glm/vec3.hpp>

class Light
{
public:
	Light();
	~Light();
	Light(glm::vec3 p, float i, float a);

	glm::vec3 getPosition() { return position; };
	float getIntensity() { return intensity; };
	float getAmbient() { return ambient; };

private:
	glm::vec3 position;
	float intensity;
	float ambient;
};

