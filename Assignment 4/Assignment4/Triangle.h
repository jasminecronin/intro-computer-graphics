// Jasmine Roebuck & Quinn Masters
// Defines a triangle object with 3 points

#pragma once
#include "Object.h"

class Triangle :
	public Object
{
public:
	Triangle();
	Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, glm::vec3 color, glm::vec3 diffuse, glm::vec3 specular, float phong, bool b);
	Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 color, glm::vec3 diffuse, glm::vec3 specular, float phong, bool b);
	virtual ~Triangle();
	virtual float intersect(Ray ray) override; // Inherited intersection method
	float get_alpha(float area, glm::vec3 poi);
	float get_beta(float area, glm::vec3 poi);
	float getT_value();
	glm::vec3 point_of_interest();

private:
	glm::vec3 point_a;
	glm::vec3 point_b; 
	glm::vec3 point_c;
	glm::vec3 origin;
	glm::vec3 direction; 
	float t_value;
};

