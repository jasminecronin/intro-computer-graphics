// Quinn Masters & Jasmine Roebuck
// Defines a normal vector for a plane and a point on that plane

#pragma once
#include "Object.h"

class Plane :
	public Object
{
public:
	Plane();
	virtual ~Plane();
	Plane(float xn, float yn, float zn, float px, float py, float pz, glm::vec3 c, glm::vec3 diffuse, glm::vec3 specular, float phong, bool b);
	virtual float intersect(Ray ray) override; // Inherited method for determining intersection point

private: 
	glm::vec3 normal;
	glm::vec3 point;
};

