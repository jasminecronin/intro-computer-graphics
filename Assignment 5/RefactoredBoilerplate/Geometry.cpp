/*
* Geometry.cpp
*
*  Created on: Sep 10, 2018
*      Author: John Hall
* 
* Edited by Jasmine Roebuck and Eddy Qiang
* December 11, 2018
*/

#include "Geometry.h"

Geometry::Geometry() : vao(0), vertexBuffer(0), normalBuffer(0), uvBuffer(0), colorBuffer(0) {
	//vectors are initially empty
	//Pointers are initially null
	//Call RenderingEngine::assignBuffers and RenderingEngine::setBufferData to fully initialize the geometry
	//Overload the constructor for more functionality or create subclasses for specific objects
}

Geometry::~Geometry() {

}

// Given a specified translation vector, translates the current object 
void Geometry::translate(float x, float y, float z)
{
	glm::mat4 translation = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		x, y, z, 1.0f);

	translationMatrix = translation * translationMatrix;
}

// Given a scale factor, uniformly scales the object
void Geometry::scale(float s) {
	glm::mat4 scale = glm::mat4(s, 0, 0, 0,
		0, s, 0, 0,
		0, 0, s, 0,
		0, 0, 0, 1);

	scalingMatrix = scale * scalingMatrix;
}

// Unfinished functions, went unused
void Geometry::rotate(float t)
{
	float theta = t * angle;
	glm::mat4 r = glm::mat4(cos(theta), 0, -sin(theta), 0,
		0, 1, 0, 0,
		sin(theta), 0, cos(theta), 0,
		0, 0, 0, 1);
	rotationMatrix = r * rotationMatrix;
}

glm::mat4 Geometry::getLocalMatrix() const
{
	glm::mat4 m = translationMatrix * rotationMatrix * scalingMatrix;
	return m;
}

glm::mat4 Geometry::getTranslationMatrix() const
{
	return translationMatrix;
}
