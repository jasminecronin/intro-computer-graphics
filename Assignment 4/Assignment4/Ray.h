// Jasmine Roebuck and Quinn Masters
// Defines a ray object with an origin and a direction

#pragma once

#ifndef RAY_H_
#define RAY_H_

#include <glm/vec3.hpp>

class Ray
{
public:
	Ray();
	virtual ~Ray();
	Ray(glm::vec3 o, glm::vec3 d);
	glm::vec3 getDirection() { return direction; };
	glm::vec3 getOrigin() { return origin; };

	glm::vec3 evaluate(float t); // point on ray evaluated at t

private:
	glm::vec3 origin;
	glm::vec3 direction;
};

#endif /* RAY_H_ */
