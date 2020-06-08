/*
* RenderingEngine.cpp
*
*  Created on: Sep 10, 2018
*      Author: John Hall
* Edited by Jasmine Roebuck and Eddy Qiang
* December 11, 2018
*/

#include "RenderingEngine.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

//included here because it just contains some global functions
#include "ShaderTools.h"

RenderingEngine::RenderingEngine() {
	shaderProgram = ShaderTools::InitializeShaders();
	if (shaderProgram == 0) {
		std::cout << "Program could not initialize shaders, TERMINATING" << std::endl;
		return;
	}
	//initialize time to 0
	t = 0;
}

RenderingEngine::~RenderingEngine() {

}

void RenderingEngine::RenderScene(const std::vector<Geometry>& objects, GLFWwindow* window, float theta, float phi, float r) {
	//Clears the screen to a dark grey background
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	float x, y, z;
	theta *= 3.14f / 180.0f;
	phi *= 3.14f / 180.0f;

	x = r * sin(theta) * sin(phi);
	y = r * cos(theta);
	z = r * sin(theta) * cos(phi);

	// Send camera position to shaders for shading calculations
	glm::vec3 eye = glm::vec3(x, y, z);
	glUseProgram(shaderProgram);
	GLuint e = glGetUniformLocation(shaderProgram, "eye");
	glUniform3fv(e, 1, &eye[0]);

	// Get Projection and View matrices
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f);
	glm::mat4 View = glm::lookAt(
		glm::vec3(x, y, z), // eye
		glm::vec3(0, 0, 0), // at
		glm::vec3(0, 1, 0)  // up
	);

	// Enable z buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Send Projection and View matrices to shader
	GLuint ViewMatrix = glGetUniformLocation(shaderProgram, "V");
	glUniformMatrix4fv(ViewMatrix, 1, GL_FALSE, &View[0][0]);

	GLuint ProjectionMatrix = glGetUniformLocation(shaderProgram, "P");
	glUniformMatrix4fv(ProjectionMatrix, 1, GL_FALSE, &Projection[0][0]);
	
	// Increase the time parameter, assumes render calls occur 60 times / second
	t += 1.0f / 60.0f;

	// Loop through list of objects to draw
	for (const Geometry& g : objects) {

		glm::vec3 y_axis = { 0,1,0 };
		//rotate about y axis, angle stored individually
		glm::mat4 rotate = glm::rotate(g.rotationMatrix, t*g.angle, y_axis);

		//orbit around the sun 
		glm::mat4 orbit = glm::rotate(g.rotationMatrix, t/5*g.angle, y_axis);

		// Send this object's texture to the shader
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g.texture.textureID);
		GLuint uniformLocation = glGetUniformLocation(shaderProgram, "imageTexture");
		glUniform1i(uniformLocation, 0);

		// Send this object's model matrix to the shader, encodes axial spin
		glm::mat4 Model = orbit * g.getLocalMatrix() * rotate;
		GLuint ModelMatrix = glGetUniformLocation(shaderProgram, "M");
		glUniformMatrix4fv(ModelMatrix, 1, GL_FALSE, &Model[0][0]);

		// Send the shading flag to the shader
		GLint flag = glGetUniformLocation(shaderProgram, "flag");
		glUniform1i(flag, g.shadingFlag);

		glBindVertexArray(g.vao);
		glDrawArrays(g.drawMode, 0, g.verts.size());

		// reset state to default (no shader or geometry bound)
		glBindVertexArray(0);
	}
	glUseProgram(0);

	//increment time
	t += 1.0f / 60.0f;

	// check for an report any OpenGL errors
	CheckGLErrors();
}

void RenderingEngine::assignBuffers(Geometry& geometry) {
	//Generate vao for the object
	//Constant 1 means 1 vao is being generated
	glGenVertexArrays(1, &geometry.vao);
	glBindVertexArray(geometry.vao);

	//Generate vbos for the object
	//Constant 1 means 1 vbo is being generated
	glGenBuffers(1, &geometry.vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBuffer);
	//Parameters in order: Index of vbo in the vao, number of primitives per element, primitive type, etc.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &geometry.normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.normalBuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &geometry.colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.colorBuffer);
	//Parameters in order: Index of vbo in the vao, number of primitives per element, primitive type, etc.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &geometry.uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.uvBuffer);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(3);
}

void RenderingEngine::setBufferData(Geometry& geometry) {
	//Send geometry to the GPU
	//Must be called whenever anything is updated about the object
	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * geometry.verts.size(), geometry.verts.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, geometry.normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * geometry.normals.size(), geometry.normals.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, geometry.colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * geometry.colors.size(), geometry.colors.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, geometry.uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * geometry.uvs.size(), geometry.uvs.data(), GL_STATIC_DRAW);
}

void RenderingEngine::deleteBufferData(Geometry& geometry) {
	glDeleteBuffers(1, &geometry.vertexBuffer);
	glDeleteBuffers(1, &geometry.normalBuffer);
	glDeleteBuffers(1, &geometry.colorBuffer);
	glDeleteBuffers(1, &geometry.uvBuffer);
	glDeleteVertexArrays(1, &geometry.vao);
}

bool RenderingEngine::CheckGLErrors() {
	bool error = false;
	for (GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError()) {
		std::cout << "OpenGL ERROR:  ";
		switch (flag) {
		case GL_INVALID_ENUM:
			std::cout << "GL_INVALID_ENUM" << std::endl; break;
		case GL_INVALID_VALUE:
			std::cout << "GL_INVALID_VALUE" << std::endl; break;
		case GL_INVALID_OPERATION:
			std::cout << "GL_INVALID_OPERATION" << std::endl; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl; break;
		case GL_OUT_OF_MEMORY:
			std::cout << "GL_OUT_OF_MEMORY" << std::endl; break;
		default:
			std::cout << "[unknown error code]" << std::endl;
		}
		error = true;
	}
	return error;
}