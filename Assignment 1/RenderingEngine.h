/*
 * RenderingEngine.h
 *
 *  Created on: Sep 10, 2018
 *      Author: John Hall
 */

#ifndef RENDERINGENGINE_H_
#define RENDERINGENGINE_H_

//**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Geometry.h"

//Forward declaration of classes
//(note this is necessary because these are pointers and it allows the #include to appear in the .cpp file)
struct GLFWwindow;

class RenderingEngine {
public:
	RenderingEngine();
	virtual ~RenderingEngine();

	//Renders each object
	void RenderScene(const std::vector<Geometry>& objects);

	//Create vao and vbos for objects
	static void assignBuffers(Geometry& geometry);
	static void setBufferData(Geometry& geometry);
	static void deleteBufferData(Geometry& geometry);

	//Ensures that vao and vbos are set up properly
	bool CheckGLErrors();

private:
	//Pointer to the current shader program being used to render
	GLuint shaderProgram;
};

#endif /* RENDERINGENGINE_H_ */
