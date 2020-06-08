// Jasmine Roebuck and Quinn Masters
// Defines a ray object with an origin and a direction

#include "Ray.h"

Ray::Ray() {
	origin = glm::vec3(0.0f, 0.0f, 0.0f);
	direction = glm::vec3(0.0f, 0.0f, 0.0f);
}

Ray::Ray(glm::vec3 o, glm::vec3 d) {
	origin = o;
	direction = d;
}


Ray::~Ray() {
}

glm::vec3 Ray::evaluate(float t) {
	return origin + (t * direction);
}
