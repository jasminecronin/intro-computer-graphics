/*
* ShaderTools.cpp
*	Tools for parsing, compiling and linking shaders
*  Created on: Sep 10, 2018
*      Author: John Hall
*/
#ifndef SHADERTOOLS_H_
#define SHADERTOOLS_H_

#include <string>

//**Must include glad and GLFW in this order or it breaks**
#include "glad/glad.h"
#include <GLFW/glfw3.h>

//Shader associated functions are put in the ShaderTools namespace
namespace ShaderTools {

	std::string LoadSource(const std::string &filename);

	// creates and returns a shader object compiled from the given source
	GLuint CompileShader(GLenum shaderType, const std::string &source);

	// creates and returns a program object linked from vertex and fragment shaders
	GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);

	GLuint InitializeShaders();
}

#endif /* SHADERTOOLS_H_ */