/*
* Geometry.h
*	Class for storing geometry
*  Created on: Sep 10, 2018
*      Author: John Hall
* 
* Edited by Jasmine Roebuck and Eddy Qiang
* December 11, 2018
*/

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "texture.h"

class Geometry {
public:
	Geometry();
	virtual ~Geometry();

	//Data structures for storing vertices, normals, colors and uvs
	std::vector<glm::vec3> verts;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec2> uvs;

	MyTexture texture; 
	int shadingFlag = 1; // Determines if object will be shaded

	glm::mat4 translationMatrix = glm::mat4(1.0f); // Initialize local matrix to an identity matrix
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	glm::mat4 scalingMatrix = glm::mat4(1.0f);

	float angle = 0; // radians per frame
	glm::vec3 axis; // local axis of rotation

	//Pointers to the vao and vbos associated with the geometry
	GLuint vao;
	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint uvBuffer;
	GLuint colorBuffer;

	// Only used for initializing the position of the celestial body
	void translate(float x, float y, float z);
	void scale(float s);
	void rotate(float t);

	// I think these went unused
	glm::mat4 getLocalMatrix() const;
	glm::mat4 getTranslationMatrix() const;


	//Draw mode for how OpenGL interprets primitives
	GLuint drawMode;
};

#endif /* GEOMETRY_H_ */