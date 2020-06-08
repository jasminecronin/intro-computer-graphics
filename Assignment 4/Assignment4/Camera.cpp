// Quinn Master & Jasmine Roebuck
// Defines an origin point for a camera

#include "Camera.h"

Camera::Camera(){
	Origin = glm::vec3(0.0, 0.0, 0.0);
}

Camera::~Camera()
{
}

void Camera::setOrigin(float x, float y, float z){
	Origin = glm::vec3(x, y, z); 
}

