// Jasmine Roebuck & Quinn Masters
// Generic superclass for defining 3D objects

#pragma once
#include "Ray.h"
#include "Light.h"

class Object
{
public:
	Object();
	// Inherited methods for subclass types
	virtual ~Object();
	virtual float intersect(Ray viewray);
	// math functions
	float dotProduct(glm::vec3 vector1, glm::vec3 vector2);
	glm::vec3 cross_product(glm::vec3 vectorA, glm::vec3 vectorB, glm::vec3 vectorC);

	// Getters
	glm::vec3 getColor() { return color; };
	glm::vec3 getShading(glm::vec3 n, glm::vec3 l, glm::vec3 v, Light light);
	glm::vec3 getShadow(Light light);
	glm::vec3 getSpecular() { return ks; };
	glm::vec3 getNormal() { return normal; };
	bool isReflective() { return reflective; };

	// Setters
	void setColor(glm::vec3 c);
	void setDiffuse(glm::vec3 diffuse);
	void setSpecular(glm::vec3 specular);
	void setNormal(glm::vec3 norm);
	void setPhong(float p);
	void setReflective(bool b);


private:
	glm::vec3 normal; // Surface normal, not constant for a sphere
	glm::vec3 color; // Material color
	glm::vec3 ka; // Ambient color
	glm::vec3 kd; // Diffuse color
	glm::vec3 ks; // Specular color
	float phong; // Phong exponent
	bool reflective; // Is surface reflective
};

