// Jasmine Roebuck & Quinn Masters
// Controls the creation and rendering of all objects,
// renders up to 3 scenes using ray tracing

#pragma once

#include "imagebuffer.h"
#include "Ray.h"
#include "Camera.h"
#include "Sphere.h"
#include "Object.h"
#include "Plane.h"

class Scene
{
public:
	Scene();
	~Scene();
	void renderImage();
	void displayRadialGradient();
	float zCamera(float width, float angle);
	glm::vec3 normalize(glm::vec3 vector);
	void createViewRays();

	void Scene1();
	void Scene2();
	void Scene3();

	void buildTetrahedron(glm::vec3 color);
	void buildTriangularMirrors(glm::vec3 color);

	void traceRays();
	glm::vec3 findColor(float t, int object, int r);
	glm::vec3 reflect(Ray viewRay, glm::vec3 point, glm::vec3 normal, int object, int count);

	float dot(glm::vec3 v1, glm::vec3 v2);
	glm::vec3 cross(glm::vec3 v1, glm::vec3 v2);

private:
	ImageBuffer image;
	Camera* camera = new Camera();
	float FOV;
	std::vector <Ray> rays; // List of view rays
	std::vector <Object*> objects; // List of objects to render
	std::vector <Light> lights; // Currently only supports a single light source
};


