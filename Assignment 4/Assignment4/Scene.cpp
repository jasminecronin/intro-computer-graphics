// Jasmine Roebuck & Quinn Masters
// Controls the creation and rendering of all objects,
// renders up to 3 scenes using ray tracing

#include "Scene.h"
#include "Ray.h"
#include "Camera.h"
#include "Object.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "Light.h"

#include <iostream>
#include <numeric>
#include <string>
#include <algorithm>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Scene::Scene() {
	image.Initialize();
	FOV = 62.25; //Scene currently works best at 62.25 degree FOV
	camera->setOrigin(0.0f, 0.0f, -zCamera(1.0f, FOV / 2));
	createViewRays();
}

Scene::~Scene() {
}

void Scene::renderImage() {
	image.Render();
}

// Calculates the z-coordinate of the camera based on the given FOV angle
float Scene::zCamera(float width, float angle) {
	float z = width / tan(angle);
	return z;
}

// Generates the list of view rays upon initialization
// Window must be square to avoid stretching
void Scene::createViewRays() {
	rays.clear();
	for (unsigned int i = 0; i < image.Width(); i++) {
		for (unsigned int j = 0; j < image.Height(); j++) {
			float x = ((i - (image.Width() / 2.0f)) / (image.Width() / 2.0f)) + (1.0f / image.Width());
			float y = ((j - (image.Height() / 2.0f)) / (image.Height() / 2.0f)) +(1.0f / image.Height());
			glm::vec3 direction = glm::vec3(x, y, 0.0f);
			Ray viewRay = Ray(camera->getOrigin(), normalize(direction-camera->getOrigin())); //***
			rays.push_back(viewRay);
		}
	}
}

// Normalizes the given vector
glm::vec3 Scene::normalize(glm::vec3 vector) {
	float length = 0.0;
	length = sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	glm::vec3 result = (1 / length) * vector;
	return result;
}

// Renders the Cornell Box scene
void Scene::Scene1() {
	objects.clear();
	lights.clear();
	glm::vec3 color;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float phong;
	bool reflective;

	// Reflective sphere
	color = glm::vec3(0.6f, 0.6f, 0.6f);
	diffuse = glm::vec3(0.5, 0.5, 0.5);
	specular = glm::vec3(0.5, 0.5, 0.5);
	phong = 1000;
	reflective = true;
	Sphere* sphere = new Sphere(0.9, -1.925, -6.69, 0.825, color, diffuse, specular, phong, reflective);

	// White wall, ceiling, & floor
	color = glm::vec3(0.9f, 0.9f, 0.9f);
	diffuse = glm::vec3(0.6f, 0.6f, 0.6f);
	specular = glm::vec3(0, 0, 0);
	phong = 10;
	reflective = false;
	Plane* backWall = new Plane(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -10.5f, color, diffuse, specular, phong, reflective);
	Triangle* ceiling1 = new Triangle(2.75, 2.75, -10.5, 2.75, 2.75, -5, -2.75, 2.75, -5, color, diffuse, specular, phong, reflective);
	Triangle* ceiling2 = new Triangle(-2.75, 2.75, -10.5, 2.75, 2.75, -10.5, -2.75, 2.75, -5, color, diffuse, specular, phong, reflective);
	Triangle* floor1 = new Triangle(2.75, -2.75, -5, 2.75, -2.75, -10.5, -2.75, -2.75, -10.5, color, diffuse, specular, phong, reflective);
	Triangle* floor2 = new Triangle(-2.75, -2.75, -5, 2.75, -2.75, -5, -2.75, -2.75, -10.5, color, diffuse, specular, phong, reflective);

	// Blue pyramid
	color = glm::vec3(0.0, 0.75, 0.75);
	diffuse = glm::vec3(0.5, 0.5, 0.5);
	specular = glm::vec3(0, 0.25, 0.25);
	phong = 50;
	reflective = true;
	Triangle* pyramid1 = new Triangle(-0.4, -2.75, -9.55, -0.93, 0.55, -8.51, 0.11, -2.75, -7.98, color, diffuse, specular, phong, reflective);
	Triangle* pyramid2 = new Triangle(0.11, -2.75, -7.98, -0.93, 0.55, -8.51, -1.46, -2.75, -7.47, color, diffuse, specular, phong, reflective);
	Triangle* pyramid3 = new Triangle(-1.46, -2.75, -7.47, -0.93, 0.55, -8.51, -1.97, -2.75, -9.04, color, diffuse, specular, phong, reflective);
	Triangle* pyramid4 = new Triangle(-1.97, -2.75, -9.04, -0.93, 0.55, -8.51, -0.4, -2.75, -9.55, color, diffuse, specular, phong, reflective);
	
	// Green wall
	color = glm::vec3(0.0, 0.75, 0.0);
	diffuse = glm::vec3(0.2, 0.75, 0.2);
	specular = glm::vec3(0.25, 0.25, 0.25);
	phong = 1;
	reflective = false;
	Triangle* rightWall1 = new Triangle(2.75, 2.75, -5, 2.75, 2.75, -10.5, 2.75, -2.75, -10.5, color, diffuse, specular, phong, reflective);
	Triangle* rightWall2 = new Triangle(2.75, -2.75, -5, 2.75, 2.75, -5, 2.75, -2.75, -10.5, color, diffuse, specular, phong, reflective);

	// Red wall
	color = glm::vec3(0.75, 0.0, 0.0);
	diffuse = glm::vec3(0.75, 0.2, 0.2);
	specular = glm::vec3(0.25, 0.25, 0.25);
	phong = 1;
	reflective = false;
	Triangle* leftWall1 = new Triangle(-2.75, -2.75, -5, -2.75, -2.75, -10.5, -2.75, 2.75, -10.5, color, diffuse, specular, phong, reflective);
	Triangle* leftWall2 = new Triangle(-2.75, 2.75, -5, -2.75, -2.75, -5, -2.75, 2.75, -10.5, color, diffuse, specular, phong, reflective);

	// Add all objects to the global list
	objects.push_back(sphere);
	objects.push_back(backWall);
	objects.push_back(pyramid1);
	objects.push_back(pyramid2);
	objects.push_back(pyramid3);
	objects.push_back(pyramid4);
	objects.push_back(ceiling1);
	objects.push_back(ceiling2);
	objects.push_back(rightWall1);
	objects.push_back(rightWall2);
	objects.push_back(leftWall1);
	objects.push_back(leftWall2);
	objects.push_back(floor1);
	objects.push_back(floor2);

	Light light = Light(glm::vec3(0, 2.5, -7.75), 0.65, 0.5); // position, intensity, ambient
	lights.push_back(light);

	traceRays();
}

// Renders the icosahedron scene
void Scene::Scene2() {
	objects.clear();
	lights.clear();
	glm::vec3 color;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float phong;
	bool reflective;

	// Large yellow sphere
	color = glm::vec3(1.0f, 1.0f, 0.0f);
	diffuse = glm::vec3(1.0, 1.0, 1.0);
	specular = glm::vec3(0.5, 0.5, 0.5);
	phong = 8;
	reflective = false;
	Sphere* sphere1 = new Sphere(1, -0.5, -3.5, 0.5, color, diffuse, specular, phong, reflective);

	// Reflective grey sphere
	color = glm::vec3(0.6f, 0.6f, 0.6f);
	diffuse = glm::vec3(0.6, 0.6, 0.6);
	specular = glm::vec3(0.8, 0.8, 0.8);
	phong = 100;
	reflective = true;
	Sphere* sphere2 = new Sphere(0, 1, -5, 0.4, color, diffuse, specular, phong, reflective); 

	// Metallic purple sphere
	color = glm::vec3(1.0f, 0.0f, 1.0f);
	diffuse = glm::vec3(0.5, 0.5, 0.5);
	specular = glm::vec3(0.25, 0, 0.25);
	phong = 80;
	reflective = true;
	Sphere* sphere3 = new Sphere(-0.8, -0.75, -4, 0.25, color, diffuse, specular, phong, reflective);

	// White floor
	color = glm::vec3(0.9f, 0.9f, 0.9f);
	diffuse = glm::vec3(0.75f, 0.75f, 0.75f);
	specular = glm::vec3(0.25, 0.25, 0.25);
	phong = 10;
	reflective = false;
	Plane* floor = new Plane(0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, color, diffuse, specular, phong, reflective);

	// Cyan background
	color = glm::vec3(0.0f, 0.75f, 0.75f);
	diffuse = glm::vec3(0.25, 0.25, 0.25);
	specular = glm::vec3(0.5, 0.5, 0.5);
	phong = 50;
	reflective = false;
	Plane* backWall = new Plane(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -12.0f, color, diffuse, specular, phong, reflective);

	// Green triangle cone
	color = glm::vec3(0.0, 1.0, 0.0);
	diffuse = glm::vec3(0.75, 0.75, 0.75);
	specular = glm::vec3(0, 0, 0);
	phong = 1;
	reflective = false;
	Triangle* cone1 = new Triangle(0, -1, -5.8, 0, 0.6, -5, 0.4, -1, -5.693, color, diffuse, specular, phong, reflective);
	Triangle* cone2 = new Triangle(0.4, -1, -5.693, 0, 0.6, -5, 0.6928, -1, -5.4, color, diffuse, specular, phong, reflective);
	Triangle* cone3 = new Triangle(0.6928, -1, -5.4, 0, 0.6, -5, 0.8, -1, -5, color, diffuse, specular, phong, reflective);
	Triangle* cone4 = new Triangle(0.8, -1, -5, 0, 0.6, -5, 0.6928, -1, -4.6, color, diffuse, specular, phong, reflective);
	Triangle* cone5 = new Triangle(0.6928, -1, -4.6, 0, 0.6, -5, 0.4, -1, -4.307, color, diffuse, specular, phong, reflective);
	Triangle* cone6 = new Triangle(0.4, -1, -4.307, 0, 0.6, -5, 0, -1, -4.2, color, diffuse, specular, phong, reflective);
	Triangle* cone7 = new Triangle(0, -1, -4.2, 0, 0.6, -5, -0.4, -1, -4.307, color, diffuse, specular, phong, reflective);
	Triangle* cone8 = new Triangle(-0.4, -1, -4.307, 0, 0.6, -5, -0.6928, -1, -4.6, color, diffuse, specular, phong, reflective);
	Triangle* cone9 = new Triangle(-0.6928, -1, -4.6, 0, 0.6, -5, -0.8, -1, -5, color, diffuse, specular, phong, reflective);
	Triangle* cone10 = new Triangle(-0.8, -1, -5, 0, 0.6, -5, -0.6928, -1, -5.4, color, diffuse, specular, phong, reflective);
	Triangle* cone11 = new Triangle(-0.6928, -1, -5.4, 0, 0.6, -5, -0.4, -1, -5.693, color, diffuse, specular, phong, reflective);
	Triangle* cone12 = new Triangle(-0.4, -1, -5.693, 0, 0.6, -5, 0, -1, -5.8, color, diffuse, specular, phong, reflective);

	// Red icosahedron
	color = glm::vec3(1.0, 0.0, 0.0);
	diffuse = glm::vec3(1, 0.5, 0.5);
	specular = glm::vec3(0.5, 0, 0);
	phong = 10;
	reflective = true;
	Triangle* die1 = new Triangle(-2, -1, -7, -1.276, -0.4472, -6.474, -2.276, -0.4472, -6.149, color, diffuse, specular, phong, reflective);
	Triangle* die2 = new Triangle(-1.276, -0.4472, -6.474, -2, -1, -7, -1.276, -0.4472, -7.526, color, diffuse, specular, phong, reflective);
	Triangle* die3 = new Triangle(-2, -1, -7, -2.276, -0.4472, -6.149, -2.894, -0.4472, -7, color, diffuse, specular, phong, reflective);
	Triangle* die4 = new Triangle(-2, -1, -7, -2.894, -0.4472, -7, -2.276, -0.4472, -7.851, color, diffuse, specular, phong, reflective);
	Triangle* die5 = new Triangle(-2, -1, -7, -2.276, -0.4472, -7.851, -1.276, -0.4472, -7.526, color, diffuse, specular, phong, reflective);
	Triangle* die6 = new Triangle(-1.276, -0.4472, -6.474, -1.276, -0.4472, -7.526, -1.106, 0.4472, -7, color, diffuse, specular, phong, reflective);
	Triangle* die7 = new Triangle(-2.276, -0.4472, -6.149, -1.276, -0.4472, -6.474, -1.724, 0.4472, -6.149, color, diffuse, specular, phong, reflective);
	Triangle* die8 = new Triangle(-2.894, -0.4472, -7, -2.276, -0.4472, -6.149, -2.724, 0.4472, -6.474, color, diffuse, specular, phong, reflective);
	Triangle* die9 = new Triangle(-2.276, -0.4472, -7.851, -2.894, -0.4472, -7, -2.724, 0.4472, -7.526, color, diffuse, specular, phong, reflective);
	Triangle* die10 = new Triangle(-1.276, -0.4472, -7.526,	-2.276, -0.4472, -7.851, -1.724, 0.4472, -7.851, color, diffuse, specular, phong, reflective);
	Triangle* die11 = new Triangle(-1.276, -0.4472, -6.474, -1.106, 0.4472, -7,	-1.724, 0.4472, -6.149, color, diffuse, specular, phong, reflective);
	Triangle* die12 = new Triangle(-2.276, -0.4472, -6.149, -1.724, 0.4472, -6.149, -2.724, 0.4472, -6.474, color, diffuse, specular, phong, reflective);
	Triangle* die13 = new Triangle(-2.894, -0.4472, -7, -2.724, 0.4472, -6.474,	-2.724, 0.4472, -7.526, color, diffuse, specular, phong, reflective);
	Triangle* die14 = new Triangle(-2.276, -0.4472, -7.851, -2.724, 0.4472, -7.526,	-1.724, 0.4472, -7.851, color, diffuse, specular, phong, reflective);
	Triangle* die15 = new Triangle(-1.276, -0.4472, -7.526,	-1.724, 0.4472, -7.851,	-1.106, 0.4472, -7, color, diffuse, specular, phong, reflective);
	Triangle* die16 = new Triangle(-1.724, 0.4472, -6.149, -1.106, 0.4472, -7, -2, 1, -7, color, diffuse, specular, phong, reflective);
	Triangle* die17 = new Triangle(-2.724, 0.4472, -6.474, -1.724, 0.4472, -6.149, -2, 1, -7, color, diffuse, specular, phong, reflective);
	Triangle* die18 = new Triangle(-2.724, 0.4472, -7.526, -2.724, 0.4472, -6.474, -2, 1, -7, color, diffuse, specular, phong, reflective);
	Triangle* die19 = new Triangle(-1.724, 0.4472, -7.851, -2.724, 0.4472, -7.526, -2, 1, -7, color, diffuse, specular, phong, reflective);
	Triangle* die20 = new Triangle(-1.106, 0.4472, -7, -1.724, 0.4472, -7.851, -2, 1, -7, color, diffuse, specular, phong, reflective);

	// Add all objects to the global list
	objects.push_back(sphere1);
	objects.push_back(sphere2);
	objects.push_back(sphere3);
	objects.push_back(floor);
	objects.push_back(backWall);
	objects.push_back(cone1);
	objects.push_back(cone2);
	objects.push_back(cone3);
	objects.push_back(cone4);
	objects.push_back(cone5);
	objects.push_back(cone6);
	objects.push_back(cone7);
	objects.push_back(cone8);
	objects.push_back(cone9);
	objects.push_back(cone10);
	objects.push_back(cone11);
	objects.push_back(cone12);
	objects.push_back(die1);
	objects.push_back(die2);
	objects.push_back(die3);
	objects.push_back(die4);
	objects.push_back(die5);
	objects.push_back(die6);
	objects.push_back(die7);
	objects.push_back(die8);
	objects.push_back(die9);
	objects.push_back(die10);
	objects.push_back(die11);
	objects.push_back(die12);
	objects.push_back(die13);
	objects.push_back(die14);
	objects.push_back(die15);
	objects.push_back(die16);
	objects.push_back(die17);
	objects.push_back(die18);
	objects.push_back(die19);
	objects.push_back(die20);

	Light light = Light(glm::vec3(4, 6, -1), 0.75, 0.4); // position, intensity, ambient
	lights.push_back(light);

	traceRays();
}

// Renders the custom scene
void Scene::Scene3() {
	objects.clear();
	lights.clear();

	// Color palette
	glm::vec3 plum = glm::vec3(0.255, 0.062, 0.204);
	glm::vec3 purple = glm::vec3(0.553, 0.176, 0.4);
	glm::vec3 mauve = glm::vec3(0.722, 0.569, 0.757);
	glm::vec3 babyblue = glm::vec3(0.678, 0.824, 0.973);
	glm::vec3 periwinkle = glm::vec3(0.455, 0.529, 0.847); 
	glm::vec3 blackberry = glm::vec3(0.075, 0.078, 0.086);
	glm::vec3 lavender = glm::vec3(0.631, 0.549, 0.722); 
	glm::vec3 fluff = glm::vec3(0.878, 0.827, 0.937);
	glm::vec3 cream = glm::vec3(0.941, 0.898, 0.863);
	glm::vec3 taupe = glm::vec3(0.4, 0.349, 0.349);

	glm::vec3 color;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float phong;
	bool reflective;

	Light light = Light(glm::vec3(0, 2.5, -7.75), 1.0, 0.5); // position, intensity, ambient
	
	// Reflective sphere
	color = fluff;
	diffuse = glm::vec3(0.75, 0.75, 0.75);
	specular = glm::vec3(0.5, 0.5, 0.5);
	phong = 1000;
	reflective = true;
	Sphere* sphere1 = new Sphere(1.75, 0, -7.69, 0.825, color, diffuse, specular, phong, reflective);

	// Behind camera sphere
	color = plum;
	diffuse = glm::vec3(0.75, 0.75, 0.75);
	specular = glm::vec3(0.2, 0.2, 0.2);
	phong = 10;
	reflective = false;
	Sphere* sphere2 = new Sphere(-1, 1, 5., 0.825, color, diffuse, specular, phong, reflective);

	// Walls & floor
	color = cream;
	diffuse = glm::vec3(0.6f, 0.6f, 0.6f);
	specular = glm::vec3(0, 0, 0);
	phong = 1;
	reflective = false;
	Triangle* ceiling1 = new Triangle(2.75, 2.75, -10.5, 2.75, 2.75, -5, -2.75, 2.75, -5, color, diffuse, specular, phong, reflective);
	Triangle* ceiling2 = new Triangle(-2.75, 2.75, -10.5, 2.75, 2.75, -10.5, -2.75, 2.75, -5, color, diffuse, specular, phong, reflective);
	color = babyblue;
	Triangle* rightWall2 = new Triangle(2.75, -2.75, -5, 2.75, 2.75, -5, 2.75, -2.75, -10.5, color, diffuse, specular, phong, reflective);
	Triangle* rightWall1 = new Triangle(2.75, 2.75, -5, 2.75, 2.75, -10.5, 2.75, -2.75, -10.5, color, diffuse, specular, phong, reflective);
	color = blackberry;
	diffuse = blackberry;
	specular = glm::vec3(0.5, 0.5, 0.5);
	phong = 1000;
	reflective = false;
	Triangle* floor1 = new Triangle(2.75, -2.75, -5, 2.75, -2.75, -10.5, -2.75, -2.75, -10.5, color, diffuse, specular, phong, reflective);
	Triangle* floor2 = new Triangle(-2.75, -2.75, -5, 2.75, -2.75, -5, -2.75, -2.75, -10.5, color, diffuse, specular, phong, reflective);

	// Blue sphere
	color = periwinkle;
	diffuse = glm::vec3(0.75, 0.75, 0.75);
	specular = glm::vec3(0.5, 0.5, 0.5);
	phong = 100;
	reflective = false;
	Sphere* sphere3 = new Sphere(-2, -2.25, -8, 0.5, color, diffuse, specular, phong, reflective);

	// Mirrored back wall
	color = taupe;
	diffuse = glm::vec3(0.2, 0.2, 0.2);
	specular = glm::vec3(1, 1, 1);
	phong = 10000;
	reflective = true;
	Plane* backWall = new Plane(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -10.5f, color, diffuse, specular, phong, reflective);

	// Purple left wall
	color = purple;
	diffuse = glm::vec3(0.5, 0.5, 0.5);
	specular = glm::vec3(0.25, 0.25, 0.25);
	phong = 1;
	reflective = false;
	Triangle* leftWall1 = new Triangle(-2.75, -2.75, -5, -2.75, -2.75, -10.5, -2.75, 2.75, -10.5, color, diffuse, specular, phong, reflective);
	Triangle* leftWall2 = new Triangle(-2.75, 2.75, -5, -2.75, -2.75, -5, -2.75, 2.75, -10.5, color, diffuse, specular, phong, reflective);

	buildTriangularMirrors(mauve); // Mirrored triangle objects
	buildTetrahedron(lavender); // Triangular pyramid

	// Add all objects to the global list
	objects.push_back(sphere1);
	objects.push_back(sphere2);
	objects.push_back(sphere3);
	objects.push_back(backWall);
	objects.push_back(ceiling1);
	objects.push_back(ceiling2);
	objects.push_back(rightWall1);
	objects.push_back(rightWall2);
	objects.push_back(leftWall1);
	objects.push_back(leftWall2);
	objects.push_back(floor1);
	objects.push_back(floor2);
	lights.push_back(light);

	traceRays();
}

// Creates a tetrahedron object
void Scene::buildTetrahedron(glm::vec3 color) {
	glm::vec3 diffuse = glm::vec3(0.431, 0.449, 0.522);
	glm::vec3 specular = glm::vec3(0.2, 0.2, 0.2);
	float phong = 100;
	bool reflective = true;

	Triangle* triangle1 = new Triangle(0.5, -1.5, -9, -0.25, -2.5, -9.25, 0.75, -2.5, -9, color, diffuse, specular, phong, reflective);
	Triangle* triangle2 = new Triangle(0.5, -1.5, -9, 0.75, -2.5, -9, 0, -2.5, -8, color, diffuse, specular, phong, reflective);
	Triangle* triangle3 = new Triangle(0.5, -1.5, -9, 0, -2.5, -8, -0.25, -2.5, -9.25, color, diffuse, specular, phong, reflective);

	objects.push_back(triangle1);
	objects.push_back(triangle2);
	objects.push_back(triangle3);
}

// Creates 2 mirrored triangle shards
void Scene::buildTriangularMirrors(glm::vec3 color) {
	glm::vec3 diffuse = color;
	glm::vec3 specular = glm::vec3(1,1,1);
	float phong = 10;
	bool reflective = true;

	Triangle* mirror = new Triangle(2, -2.75, -6, -2.25, 2.75, -6, -2.75, 2.75, -6, color, diffuse, specular, phong, reflective);
	Triangle* mirror2 = new Triangle(-2, -2.75, -8, 2.25, 2.75, -8, 2.75, 2.75, -8, color, diffuse, specular, phong, reflective);
	objects.push_back(mirror);
	objects.push_back(mirror2);
}

// Traces all view rays to determine pixel colors
void Scene::traceRays() {
	for (unsigned int i = 0; i < image.Width(); i++) {
		for (unsigned int j = 0; j < image.Height(); j++) {
			int obj_num = 0;
			float t_intersect = pow(2, 10);
			int rayIndex = i * image.Height() + j;
			for (unsigned int k = 0; k < objects.size(); k++) {
				float new_t = objects.at(k)->intersect(rays.at(rayIndex));
				if (new_t > 0 && new_t < t_intersect) { // Find the smallest positive t value
					t_intersect = new_t;
					obj_num = k;
				}
			}
			if (t_intersect <= 0) {
				image.SetPixel(i, j, glm::vec3(0.0, 0.0, 0.0)); // Set to background color if no intersection
			}
			else { // Find color of intersected object
				glm::vec3 color = findColor(t_intersect, obj_num, rayIndex);
				image.SetPixel(i, j, color);
			}
		}
	}
}

// Given a point of intersection with an object, determines the color to draw
glm::vec3 Scene::findColor(float t, int object, int r) {
	glm::vec3 trueColor;

	glm::vec3 intersectionPoint = rays.at(r).getOrigin() + t * rays.at(r).getDirection();
	glm::vec3 lightRay = normalize(lights.at(0).getPosition() - intersectionPoint); // Only works for one light source right now
	glm::vec3 viewRay = normalize(intersectionPoint - rays.at(r).getOrigin());
	glm::vec3 surfaceNormal = normalize(objects.at(object)->getNormal());
	Ray shadowRay = Ray(intersectionPoint, lightRay); // For shadow tracing

	float light_t = (dot(lights.at(0).getPosition() - intersectionPoint, lightRay));
	float t_value = 0;
	int obj_num = object;
	float min_value = light_t;
	// Determine if the point of intersection is shadowed by another object
	for (unsigned int k = 0; k < objects.size(); k++) {
		float compare_t = objects.at(k)->intersect(shadowRay);
		if (compare_t > 0 && k!= object) {
			if (compare_t < min_value) {
				min_value = t_value = compare_t;
				obj_num = k;
			}
		}
	}

	if (obj_num == object) { // If object is occluded by itself, get shading
		trueColor = objects.at(object)->getShading(surfaceNormal, lightRay, viewRay, lights.at(0));
	} else { // Point is in shadow, get shadow color
		trueColor = objects.at(object)->getShadow(lights.at(0));
	}

	// Get Reflected color if object is reflective
	glm::vec3 reflectedColor = glm::vec3(1, 1, 1);
	if (objects.at(object)->isReflective()) {
		reflectedColor = reflect(rays.at(r), intersectionPoint, surfaceNormal, object, 0); 
		float reflectionLevel = (objects.at(object)->getSpecular().x + objects.at(object)->getSpecular().y + objects.at(object)->getSpecular().z) / 3.0f;
		trueColor = ((1 - reflectionLevel) * trueColor) + (reflectionLevel * reflectedColor); // Mix reflected color with surface color
	}
	
	return trueColor;
}

// Recursively gets reflected colors from reflective surfaces, up to 10 light ray bounces
glm::vec3 Scene::reflect(Ray viewRay, glm::vec3 viewRayIntersectionPoint, glm::vec3 normal, int currentObject, int bounce) {
	bounce++;
	glm::vec3 reflectionDirection = viewRay.getDirection() - (2 * dot(viewRay.getDirection(), normal) * normal);
	Ray reflectedRay = Ray(viewRayIntersectionPoint, reflectionDirection);

	// Determine if reflected ray intersects any object
	int obstructingObject = currentObject;
	float min_reflected_t = pow(2, 10);
	for (unsigned int k = 0; k < objects.size(); k++) {
		float compare_t = objects.at(k)->intersect(reflectedRay);
		if (compare_t > 0 && k != currentObject)
		{
			if (compare_t < min_reflected_t)
			{
				min_reflected_t = compare_t;
				obstructingObject = k;
			}
		}
	}

	glm::vec3 reflectedRayIntersectionPoint = reflectedRay.getOrigin() + (min_reflected_t * reflectedRay.getDirection());
	glm::vec3 reflectedNormal = normalize(objects.at(obstructingObject)->getNormal());
	glm::vec3 lightOnObstruction = normalize(lights.at(0).getPosition() - reflectedRayIntersectionPoint);
	glm::vec3 reflectionAsViewRay = normalize(reflectedRayIntersectionPoint - reflectedRay.getOrigin());
	Ray shadowRay = Ray(reflectedRayIntersectionPoint, lightOnObstruction);

	// Determine if reflected point of intersection is in shadow, or on another reflective surface
	int nextObstructingObject = obstructingObject;
	glm::vec3 returningColor;
	if (obstructingObject == currentObject) {
		returningColor = objects.at(currentObject)->getShading(reflectedNormal, lightOnObstruction, reflectionDirection, lights.at(0));
	}
	else if (bounce > 10 || !objects.at(obstructingObject)->isReflective()) {
		float light_t = (dot(lights.at(0).getPosition() - reflectedRayIntersectionPoint, lightOnObstruction));
		float min_value = light_t;
		for (unsigned int k = 0; k < objects.size(); k++) {
			float compare_t = objects.at(k)->intersect(shadowRay);
			if (compare_t > 0 && k != obstructingObject) {
				if (compare_t < min_value) {
					min_value = compare_t;
					nextObstructingObject = k;
				}
			}
		}
		if (nextObstructingObject == obstructingObject) {
			returningColor = objects.at(obstructingObject)->getShading(reflectedNormal, lightOnObstruction, reflectionAsViewRay, lights.at(0));
		} else {
			returningColor = objects.at(obstructingObject)->getShadow(lights.at(0));
		}
	}
	else { // Obstructing Object is reflective
		returningColor = reflect(reflectedRay, reflectedRayIntersectionPoint, reflectedNormal, obstructingObject, bounce);
	}
	return returningColor;
}

// Calculates the dot product of 2 vectors
float Scene::dot(glm::vec3 v1, glm::vec3 v2) {
	float sum = v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
	return sum;
}

// Calculates the cross product of 2 vectors
glm::vec3 Scene::cross(glm::vec3 v1, glm::vec3 v2) {
	float i = (v1.y*v2.z) - (v1.z*v2.y);
	float j = -1 * ((v1.x*v2.z) - (v1.z*v2.x));
	float k = (v1.x*v2.y) - (v1.y*v2.x);
	return glm::vec3(i, j, k);
}


