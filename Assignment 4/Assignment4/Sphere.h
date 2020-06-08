// Jasmine Roebuck & Quinn Masters
// Defines a sphere object with a center and a radius

#pragma once
#include "Object.h"

class Sphere :
	public Object
{
public:
	Sphere();
	virtual ~Sphere();
	virtual float intersect(Ray ray) override;
	Sphere(float x, float y, float z, float r, glm::vec3 c , glm::vec3 diffuse, glm::vec3 specular, float phong, bool b);

private: 
	glm::vec3 center;
	glm::vec3 d; // Direction of test ray
	glm::vec3 o; // origin of test ray
	float radius;
	float discrim();
};

