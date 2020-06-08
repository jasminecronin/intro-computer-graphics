// Quinn Masters & Jasmine Roebuck
// Defines an origin point for a camera object

#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

class Camera
{
public:
	Camera();
	glm::vec3 getOrigin() { return Origin; };
	void setOrigin(float x, float y, float z);
	~Camera();

private: 
	glm::vec3 Origin;
};


