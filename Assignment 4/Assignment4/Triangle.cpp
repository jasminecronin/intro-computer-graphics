// Jasmine Roebuck & Quinn Masters
// Defines a triangle object with 3 points

#include "Triangle.h"
#include <algorithm>

Triangle::Triangle() {
}

Triangle::~Triangle() {
}

Triangle::Triangle(float x1, float y1, float z1, float  x2, float y2, float z2, float  x3, float y3, float z3, glm::vec3 color, glm::vec3 diffuse, glm::vec3 specular, float phong, bool b) {
	point_a = glm::vec3(x1, y1, z1);
	point_b = glm::vec3(x2, y2, z2);
	point_c = glm::vec3(x3, y3, z3);
	setColor(color);
	setNormal(cross_product(point_a, point_b, point_c));
	setDiffuse(diffuse);
	setSpecular(specular);
	setPhong(phong);
	setReflective(b);
}

// Defines a triangle object using 3 vectors instead of 9 coordinates
Triangle::Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 color, glm::vec3 diffuse, glm::vec3 specular, float phong, bool b) {
	point_a = v1;
	point_b = v2;
	point_c = v3;
	setColor(color);
	setNormal(cross_product(point_a, point_b, point_c));
	setDiffuse(diffuse);
	setSpecular(specular);
	setPhong(phong);
	setReflective(b);
}

// Given a ray, determines if that ray intersects inside the triangle
float Triangle::intersect(Ray ray) {
	direction = ray.getDirection();
	origin = ray.getOrigin();
	t_value = getT_value();
	glm::vec3 point_p = point_of_interest();
	float Area = dotProduct(getNormal(),getNormal());
	float alpha = get_alpha(Area, point_p);
	float beta = get_beta(Area, point_p);
	float gamma = 1 - alpha - beta;
	if ((0 < alpha) && (0 < beta) && (0 < gamma)) {
		return t_value;
	} else {
		return -1;
	}

}

// Functions to calculate barycentric coordinates
float Triangle::get_alpha(float area, glm::vec3 poi) {
	glm::vec3 cross = cross_product( poi,point_b,point_c);
	float part_area = dotProduct(getNormal(), cross);
	return (part_area / area);
}

float Triangle::get_beta(float area, glm::vec3 poi) {
	glm::vec3 cross = cross_product(poi, point_c, point_a);
	float part_area = dotProduct(getNormal(), cross);
	return (part_area / area);
}


float Triangle::getT_value() {
	float chunk1 = dotProduct((point_a - origin), getNormal());
	float chunk2 = dotProduct(direction, getNormal());

	float t = chunk1 / chunk2;
	if (t >= 0) {
		return t;
	}
	return -1;
}

glm::vec3 Triangle::point_of_interest() {
	return origin + (t_value*direction);
}
