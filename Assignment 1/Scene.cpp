/*
 * Scene.cpp
 *
 *  Created on: Sep 10, 2018
 *  Author: John Hall
 * 
 * 	Modified by Jasmine Roebuck, 30037334
 * 	Sep 25, 2018
 */

#include "Scene.h"

#include "RenderingEngine.h"
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <math.h>

int level = 1; // Level of nesting or recursion in the scene
std::string scene; // Scene identifier

// Increase the level of nesting or recursion
void Scene::incLevel() {
	if (level < 8) {
		level++;
	}
	// Redraw the scene
	if (scene == "diamond") {
		changeToDiamondScene();
	} else if (scene == "spiral") {
		changeToSpiralScene();
	} else if (scene == "triangle") {
		changeToTriangleScene();
	} else if (scene == "itertriangle") {
		changeToIterTriangleScene();
	} else if (scene == "fern") {
		changeToFernScene();
	} else if (scene == "hilbert") {
		changeToHilbertScene();
	}
	
}

// Decrease the level of nesting or recursion
void Scene::decLevel() {
	if (level > 1) {
		level--;
	}
	// Redraw the scene
	if (scene == "diamond") {
		changeToDiamondScene();
	} else if (scene == "spiral") {
		changeToSpiralScene();
	} else if (scene == "triangle") {
		changeToTriangleScene();
	} else if (scene == "itertriangle") {
		changeToIterTriangleScene();
	} else if (scene == "fern") {
		changeToFernScene();
	} else if (scene == "hilbert") {
		changeToHilbertScene();
	}
}

Scene::Scene(RenderingEngine* renderer) : renderer(renderer) {
	level = 1;
	changeToDiamondScene();
}

Scene::~Scene() {

}


void Scene::displayScene() {
	renderer->RenderScene(objects);
}

// Controller for the nesting squares and diamonds scene
void Scene::changeToDiamondScene() {
	scene = "diamond";
	objects.clear();

	// Set all starting x and y coordinates
	float x1 = -0.75;
	float x2 = 0.75;
	float x3 = 0.75;
	float x4 = -0.75;
	float y1 = 0.75;
	float y2 = 0.75;
	float y3 = -0.75;
	float y4 = -0.75;
	drawSquare(x1, x2, x3, x4, y1, y2, y3, y4, level * 2); // Recursive call

}

// Recursive function for drawing the squares and diamonds
void Scene::drawSquare(float x1, float x2, float x3, float x4, 
						float y1, float y2, float y3, float y4, int i) {

	if (i == 0) {return;}
	float shift = (2 * level - i) * 0.05;
	Geometry square;
	// Create the square vertices
	square.verts.push_back(glm::vec3(x1, y1, 1.0f));
	square.verts.push_back(glm::vec3(x2, y2, 1.0f));
	square.verts.push_back(glm::vec3(x3, y3, 1.0f));
	square.verts.push_back(glm::vec3(x4, y4, 1.0f));

	// Assign colors
	if (i % 2 == 0) { // Square color
		square.colors.push_back(glm::vec3(0.667f + shift, 0.365f + shift, 0.82f + shift));
		square.colors.push_back(glm::vec3(0.667f + shift, 0.365f + shift, 0.82f + shift));
		square.colors.push_back(glm::vec3(0.667f + shift, 0.365f + shift, 0.82f + shift));
		square.colors.push_back(glm::vec3(0.667f + shift, 0.365f + shift, 0.82f + shift));
	} else { // Diamond color
		square.colors.push_back(glm::vec3(1.0f, 1.0f, 0.6f + shift));
		square.colors.push_back(glm::vec3(1.0f, 1.0f, 0.6f + shift));
		square.colors.push_back(glm::vec3(1.0f, 1.0f, 0.6f + shift));
		square.colors.push_back(glm::vec3(1.0f, 1.0f, 0.6f + shift));
	}

	square.drawMode = GL_LINE_LOOP;
	RenderingEngine::assignBuffers(square);
	RenderingEngine::setBufferData(square);
	//Add the square or diamon to the scene objects
	objects.push_back(square);

	// Calculate a new set of vertices
	float temp = x1;
	x1 = (x1 + x2) / 2;
	x2 = (x2 + x3) / 2;
	x3 = (x3 + x4) / 2;
	x4 = (x4 + temp) / 2;
	temp = y1;
	y1 = (y1 + y2) / 2;
	y2 = (y2 + y3) / 2;
	y3 = (y3 + y4) / 2;
	y4 = (y4 + temp) / 2;
	i--;

	drawSquare(x1, x2, x3, x4, y1, y2, y3, y4, i);
}

// Controller for the spiral scene
void Scene::changeToSpiralScene() {
	scene = "spiral";
	objects.clear();
	drawSpiral();
}

// Iterative function for drawing the spiral
void Scene::drawSpiral() {

	Geometry spiral;	
	int numOfLines = 75 * level; // Number of line segments to construct the curve
	float color = 0.0f; // Controls change of color values
	float du = (2 * 3.14f * level) / numOfLines;
	float factor = (1 / 6.28f) * (1.0 / level); // Controls size of the spiral
	for (float u = 0; u < (2 * level * 3.14f); u += du) {
		// Calculate new coordinates
		float xCoord = u * factor * cos(u);
		float yCoord = u * factor * sin(u);
		float zCoord = 1.0f;
		color += (1.0f / numOfLines); // Shift the color

		// Add the vertex to the collection
		spiral.verts.push_back(glm::vec3(xCoord, yCoord, zCoord));
		spiral.colors.push_back(glm::vec3(0.4, 1.0 - color, color));
	}

	spiral.drawMode = GL_LINE_STRIP;
	RenderingEngine::assignBuffers(spiral);
	RenderingEngine::setBufferData(spiral);
	// Add the spiral to the scene objects
	objects.push_back(spiral);
}

// Controller for the triangle scene
void Scene::changeToTriangleScene() {
	scene = "triangle";
	objects.clear();

	// Set starting coordinates for the triangle
	float x1 = -0.6f;
	float x2 = 0.0f;
	float x3 = 0.6f;
	float y1 = -0.4f;
	float y2 = 0.6f;
	float y3 = -0.4f;
	float red = 0.1f;
	float green = 0.1f;
	float blue = 0.1f;

	drawTriangle(x1, x2, x3, y1, y2, y3, red, green, blue, level); // Recursive call
	
}

// Recursive function for drawing the Sierpinski Triangle
void Scene::drawTriangle(float x1, float x2, float x3, float y1, float y2, float y3, float red, float green, float blue, int i) {
	
	if (i == 1) { // Base case, draw a triangle
		Geometry triangle;
		triangle.verts.push_back(glm::vec3(x1, y1, 1.0f));
		triangle.verts.push_back(glm::vec3(x2, y2, 1.0f));
		triangle.verts.push_back(glm::vec3(x3, y3, 1.0f));
		// Assign colors, each triangle is just one color
		triangle.colors.push_back(glm::vec3(red, green, blue));
		triangle.colors.push_back(glm::vec3(red, green, blue));
		triangle.colors.push_back(glm::vec3(red, green, blue));

		triangle.drawMode = GL_TRIANGLES;
		RenderingEngine::assignBuffers(triangle);
		RenderingEngine::setBufferData(triangle);
		//Add the triangle to the scene objects
		objects.push_back(triangle);
		return;
	}

	// Calculate new vertices, midpoints of the original triangle sides
	i--;
	float mx1 = (x1 + x2) / 2;
	float mx2 = (x2 + x3) / 2;
	float mx3 = (x1 + x3) / 2;
	float my1 = (y1 + y2) / 2;
	float my2 = (y2 + y3) / 2;
	float my3 = (y1 + y3) / 2;

	if (level == 2) { // Split each triangle into separate colors on the first iteration	
		drawTriangle(x1, mx1, mx3, y1, my1, my3, 0.0f, green + 0.1, 0.0f, i);
		drawTriangle(mx1, x2, mx2, my1, y2, my2, red + 0.1, 0.0f, 0.0f, i);
		drawTriangle(mx3, mx2, x3, my3, my2, y3, 0.0f, 0.0f, blue + 0.1, i);
	} else { // Shift the colors on late iterations
		drawTriangle(x1, mx1, mx3, y1, my1, my3, red, green + (0.05 * i), blue, i);
		drawTriangle(mx1, x2, mx2, my1, y2, my2, red + (0.1 * level), green, blue, i);
		drawTriangle(mx3, mx2, x3, my3, my2, y3, red, green, blue + (0.15 * i), i);
	}

}

// Controller for the triangle scene generated with points
void Scene::changeToIterTriangleScene() {
	scene = "itertriangle";
	objects.clear();

	Geometry points; // Collection of generated points
	// Add the first vertices of the outer triangle
	points.verts.push_back(glm::vec3(-0.6f, -0.4f, 1.0f));
	points.verts.push_back(glm::vec3(0.0f, 0.6f, 1.0f));
	points.verts.push_back(glm::vec3(0.6f, -0.4f, 1.0f));
	// Assign a color to each point, currently white
	points.colors.push_back(glm::vec3(1, 1, 1));
	points.colors.push_back(glm::vec3(1, 1, 1));
	points.colors.push_back(glm::vec3(1, 1, 1));

	// Choose a random value from 1 to 3
	int choosePoint = rand()%(3) + 1;
	// x and y value of current point
	float x;
	float y;

	// Generate the first random point
	if (choosePoint == 1) { // Lower left vertex
		x = -0.6f;
		y = -0.4f;
	} else if (choosePoint == 2) { // Top vertex
		x = 0.0f;
		y = 0.6f;
	} else { // Lower right vertex
		x = 0.6f;
		y = -0.4f;
	}

	// Generate the rest of the random points
	for (int i = 0; i < pow(4, level - 1); i++) {
		int choose = rand()%(3) + 1;
		if (choose == 1) { // Lower left vertex
			x = (-0.6f + x) / 2;
			y = (-0.4f + y) / 2;
		} else if (choose == 2) { // Top vertex
			x = (0.0f + x) / 2;
			y = (0.6f + y) / 2;
		} else { // Lower right vertex
			x = (0.6f + x) / 2;
			y = (-0.4f + y) / 2;
		}
		points.verts.push_back(glm::vec3(x, y, 1.0f)); // Add point to the collection
		points.colors.push_back(glm::vec3(1.0f / choose, 1.0f / choose, 1.0f)); // Assign the color
	}
	
	points.drawMode = GL_POINTS;
	RenderingEngine::assignBuffers(points);
	RenderingEngine::setBufferData(points);
	// Draw the collection of points
	objects.push_back(points);
 
}

void Scene::changeToFernScene() {
	scene = "fern";
	objects.clear();

	Geometry points; // Collection of generated points
	// x and y value of current point
	float x = 0.0f;
	float y = 0.0f;
	// Add the first vertex
	points.verts.push_back(glm::vec3(x, y, 1.0f));
	// Assign a color to each point, currently green
	points.colors.push_back(glm::vec3(0, 1, 0));

	// Generate the random points
	for (int i = 0; i < pow(5, level - 1); i++) {
		// Choose a random value between 1 and 100
		int choose = rand()%(100) + 1;
		float temp = x;
		if (choose >= 1 && choose <= 85) { 
			x = (0.85 * x) + (0.04 * y);
			y = (-0.04 * temp) + (0.85 * y) + 1.6;
		} else if (choose >= 86 && choose <= 92) { 
			x = (0.2 * x) - (0.26 * y);
			y = (0.23 * temp) + (0.22 * y) + 1.6;
		} else if (choose >= 93 && choose <= 99) { 
			x = (-0.15 * x) + (0.28 * y);
			y = (0.26 * temp) + (0.24 * y) + 0.44;
		} else { // choose == 100
			x = 0.0f;
			y = 0.16 * y;
		}
		points.verts.push_back(glm::vec3((x * 0.2), (y * 0.2) - 1, 1.0f)); // Add point to the collection with a transform
		points.colors.push_back(glm::vec3(0, 1.0f, 0)); // Assign the color
	}
	
	points.drawMode = GL_POINTS;
	RenderingEngine::assignBuffers(points);
	RenderingEngine::setBufferData(points);
	// Draw the collection of points
	objects.push_back(points);
}


// Controller for the Hilbert scene
void Scene::changeToHilbertScene(){
	scene = "hilbert";
	objects.clear();

	float x = 0.0f; // Start at the origin
	float y = 0.0f;
	float length = 1.0f; // Length of each line segment
	int i = level;

	Geometry path;

	HilbertA(x, y, length, i, path);

	path.drawMode = GL_LINE_STRIP;
	RenderingEngine::assignBuffers(path);
	RenderingEngine::setBufferData(path);
	objects.push_back(path);

}

// Recursion call for Hilbert shape A. Draws an upside down cup.
void Scene::HilbertA(float x, float y, float length, int i, Geometry& path) {
	// Calculate the new set of points
	length = length * 0.5f;
	float x1 = x - length; // Lower left from origin
	float x2 = x - length; // Upper left from origin
	float x3 = x + length; // Upper right from origin
	float x4 = x + length; // Lower right from origin
	float y1 = y - length; // Lower left from origin
	float y2 = y + length; // Upper left from origin
	float y3 = y + length; // Upper right from origin
	float y4 = y - length; // Lower right from origin

	// Add these points to the collection IN THIS ORDER
	if (i == 1) {
		path.verts.push_back(glm::vec3(x1, y1, 1.0f));
		path.verts.push_back(glm::vec3(x2, y2, 1.0f));
		path.verts.push_back(glm::vec3(x3, y3, 1.0f));
		path.verts.push_back(glm::vec3(x4, y4, 1.0f));
		// Assign color
		path.colors.push_back(glm::vec3(length, 1, 1));
		path.colors.push_back(glm::vec3(length, 1, 1));
		path.colors.push_back(glm::vec3(length, 1, 1));
		path.colors.push_back(glm::vec3(length, 1, 1));
		return;
	}

	i--;
	// Draw these recursive shapes IN THIS ORDER
	HilbertB(x1, y1, length, i, path);
	HilbertA(x2, y2, length, i, path);
	HilbertA(x3, y3, length, i, path);
	HilbertD(x4, y4, length, i, path);
}

// Recursion call for Hilbert shape B. Draws a left facing cup.
void Scene::HilbertB(float x, float y, float length, int i, Geometry& path) {
	// Calculate the new set of points	
	length = length * 0.5f;
	float x1 = x - length; // Lower left from origin
	float x2 = x - length; // Upper left from origin
	float x3 = x + length; // Upper right from origin
	float x4 = x + length; // Lower right from origin
	float y1 = y - length; // Lower left from origin
	float y2 = y + length; // Upper left from origin
	float y3 = y + length; // Upper right from origin
	float y4 = y - length; // Lower right from origin

	// Add these points to the collection IN THIS ORDER
	if (i == 1) {
		path.verts.push_back(glm::vec3(x1, y1, 1.0f));
		path.verts.push_back(glm::vec3(x4, y4, 1.0f));
		path.verts.push_back(glm::vec3(x3, y3, 1.0f));
		path.verts.push_back(glm::vec3(x2, y2, 1.0f));
		// Assign color
		path.colors.push_back(glm::vec3(1, length, 1));
		path.colors.push_back(glm::vec3(1, length, 1));
		path.colors.push_back(glm::vec3(1, length, 1));
		path.colors.push_back(glm::vec3(1, length, 1));
		return;
	}

	i--;
	// Draw these recursive shapes IN THIS ORDER
	HilbertA(x1, y1, length, i, path);
	HilbertB(x4, y4, length, i, path);
	HilbertB(x3, y3, length, i, path);
	HilbertC(x2, y2, length, i, path);
}

// Recursion call for Hilbert shape C. Draws an upward cup.
void Scene::HilbertC(float x, float y, float length, int i, Geometry& path) {
	// Calculate the new set of points
	length = length * 0.5f;
	float x1 = x - length; // Lower left from origin
	float x2 = x - length; // Upper left from origin
	float x3 = x + length; // Upper right from origin
	float x4 = x + length; // Lower right from origin
	float y1 = y - length; // Lower left from origin
	float y2 = y + length; // Upper left from origin
	float y3 = y + length; // Upper right from origin
	float y4 = y - length; // Lower right from origin

	// Add these points to the collection IN THIS ORDER
	if (i == 1) {
		path.verts.push_back(glm::vec3(x3, y3, 1.0f));
		path.verts.push_back(glm::vec3(x4, y4, 1.0f));
		path.verts.push_back(glm::vec3(x1, y1, 1.0f));
		path.verts.push_back(glm::vec3(x2, y2, 1.0f));
		// Assign color
		path.colors.push_back(glm::vec3(1, 1, length));
		path.colors.push_back(glm::vec3(1, 1, length));
		path.colors.push_back(glm::vec3(1, 1, length));
		path.colors.push_back(glm::vec3(1, 1, length));
		return;
	}

	i--;
	// Draw these recursive shapes IN THIS ORDER
	HilbertD(x3, y3, length, i, path);
	HilbertC(x4, y4, length, i, path);
	HilbertC(x1, y1, length, i, path);
	HilbertB(x2, y2, length, i, path);
}

// Recursion call for Hilbert shape D. Draws a right facing cup.
void Scene::HilbertD(float x, float y, float length, int i, Geometry& path) {
	// Calculate the new set of points
	length = length * 0.5f;
	float x1 = x - length; // Lower left from origin
	float x2 = x - length; // Upper left from origin
	float x3 = x + length; // Upper right from origin
	float x4 = x + length; // Lower right from origin
	float y1 = y - length; // Lower left from origin
	float y2 = y + length; // Upper left from origin
	float y3 = y + length; // Upper right from origin
	float y4 = y - length; // Lower right from origin

	// Add these points to the collection IN THIS ORDER
	if (i == 1) {
		path.verts.push_back(glm::vec3(x3, y3, 1.0f));
		path.verts.push_back(glm::vec3(x2, y2, 1.0f));
		path.verts.push_back(glm::vec3(x1, y1, 1.0f));
		path.verts.push_back(glm::vec3(x4, y4, 1.0f));
		// Assign color
		path.colors.push_back(glm::vec3(length, 1, length));
		path.colors.push_back(glm::vec3(length, 1, length));
		path.colors.push_back(glm::vec3(length, 1, length));
		path.colors.push_back(glm::vec3(length, 1, length));
		return;
	}

	i--;
	// Draw these recursive shapes IN THIS ORDER
	HilbertC(x3, y3, length, i, path);
	HilbertD(x2, y2, length, i, path);
	HilbertD(x1, y1, length, i, path);
	HilbertA(x4, y4, length, i, path);
}





