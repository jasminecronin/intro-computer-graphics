// Quinn Masters & Jasmine Roebuck
// Defines a normal vector for a plane and a point on that plane

#include "Plane.h"

Plane::Plane()
{
}

Plane::~Plane()
{
}

Plane::Plane(float xn, float yn, float zn, float px, float py, float pz, glm::vec3 c, glm::vec3 diffuse, glm::vec3 specular, float phong, bool b) {
	setNormal(glm::vec3(xn, yn, zn));
	point = glm::vec3(px, py, pz);
	setColor(c);
	setDiffuse(diffuse);
	setSpecular(specular);
	setPhong(phong);
	setReflective(b);
}

// Given a ray, calculates if the ray will intersect with the plane and returns the t value
// to find such a point on that ray
float Plane::intersect(Ray ray) {
	glm::vec3 d = ray.getDirection();
	glm::vec3 o = ray.getOrigin();
	float numer = dotProduct((point - o), getNormal());
	float denom = dotProduct(d, getNormal());

	float t = numer / denom;
	if (t > 0) {
		return t;
	}
	return -1;
}



