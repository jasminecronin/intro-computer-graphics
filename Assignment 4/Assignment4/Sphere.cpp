// Jasmine Roebuck & Quinn Masters
// Defines a sphere object with a center and a radius

#include "Sphere.h"
#include <algorithm>

Sphere::Sphere() {
}

Sphere::~Sphere() {
}

Sphere::Sphere(float x, float y, float z, float r, glm::vec3 c, glm::vec3 diffuse, glm::vec3 specular, float phong, bool b) {
	center = glm::vec3(x, y, z);
	radius = r;
	setColor(c);
	setDiffuse(diffuse); 
	setSpecular(specular);
	setPhong(phong);
	setReflective(b);
}

// Given a ray, determines the closest point of intersection with that ray
float Sphere::intersect(Ray ray) {
	float t;
	d = ray.getDirection();
	o = ray.getOrigin();
	float compare = discrim(); // Get discriminant of quadratic equation
	if (compare == 0) { // One intersection point
		t= dotProduct(-d, o - center) / dotProduct(d, d);
		return t;
	}
	else if (compare > 0) { // Two intersection points, get one with smaller t
		float t1;
		float t2;
		t1 = (dotProduct(-d, o - center) + sqrt(compare)) / dotProduct(d, d);
		t2 = (dotProduct(-d, o - center) - sqrt(compare)) / dotProduct(d, d);
		if (t1 > 0 && t2 > 0) {
			t = std::min(t1, t2);
			setNormal((ray.getOrigin() + t * ray.getDirection()) - center);
			return t;
		} else {
			t = std::max(t1, t2);
			setNormal(ray.getOrigin() + t * ray.getDirection() - center);
			return t;
		}
	} else { // Ray does not intersect sphere
		return -1;
	}
}

// Determines the discriminant of the quadratic equation
float Sphere::discrim() {
	float chunk1 = pow(dotProduct(-d, o - center),2);
	float chunk2 = dotProduct(d, d);
	float chunk3 = (dotProduct(o - center, o - center) - pow(radius, 2));
	float result = chunk1 - chunk2*chunk3;
	return result;
}
