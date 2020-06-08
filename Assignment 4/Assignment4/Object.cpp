// Jasmine Roebuck & Quinn Masters
// Generic superclass for defining 3D objects

#include "Object.h"
#include <algorithm>
#include <iostream>

Object::Object()
{
}

Object::~Object()
{
}

float Object::intersect(Ray viewray) {
	return 0.0f;
}

// Calculates the dot product of two vectors
float Object::dotProduct(glm::vec3 vector1, glm::vec3 vector2) {
	float sum = (vector1.x * vector2.x) + (vector1.y * vector2.y) + (vector1.z * vector2.z);
	return sum;
}

// Given three vectors A, B, & C, calculates (B - A) x (C - A)
// Used for getting the normal of a triangle object
glm::vec3 Object::cross_product(glm::vec3 vectorA, glm::vec3 vectorB, glm::vec3 vectorC) {
	glm::vec3 b_a = vectorB - vectorA;
	glm::vec3 c_a = vectorC - vectorA;
	float i = (b_a.y * c_a.z) - (b_a.z * c_a.y);
	float j = -1 * ((b_a.x * c_a.z) - (b_a.z * c_a.x));
	float k = (b_a.x * c_a.y) - (b_a.y * c_a.x);
	return glm::vec3(i, j, k);
}

// Given a surface normal n, a view ray v, a light ray l, and a light object,
// calculates a color using the Phong shading model
glm::vec3 Object::getShading(glm::vec3 n, glm::vec3 l, glm::vec3 v, Light light) {
	glm::vec3 ambient = ka * light.getAmbient();
	glm::vec3 diffuse = kd * light.getIntensity() * std::max(0.0f, dotProduct(n, l));
	glm::vec3 reflectedRay = l - (2 * dotProduct(l, n) * n);
	glm::vec3 specular = ks * light.getIntensity() * pow(std::max(0.0f, dotProduct(reflectedRay, v)), phong);

	glm::vec3 L = ambient + diffuse + specular; // Final color luminance
	return L;
}

// Returns the ambient lighting color when the object is in shadow
glm::vec3 Object::getShadow(Light light) {
	return ka * light.getAmbient();
}

// Setters for object members
void Object::setColor(glm::vec3 c) {
	color = c;
	ka = color;
}

void Object::setDiffuse(glm::vec3 diffuse) {
	kd = diffuse;
}

void Object::setSpecular(glm::vec3 specular) {
	ks = specular;
}

void Object::setNormal(glm::vec3 norm) {
	normal = norm;
}

void Object::setPhong(float p) {
	phong = p;
}

void Object::setReflective(bool b) {
	reflective = b;
}




