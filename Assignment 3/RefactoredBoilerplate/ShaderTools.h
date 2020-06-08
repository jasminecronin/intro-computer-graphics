/*
 * ShaderTools.h
 *
 *  Created on: Sep 17, 2018
 *      Author: cb-ha
 */

#ifndef SHADERTOOLS_H_
#define SHADERTOOLS_H_

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace ShaderTools {

	std::string LoadSource(const std::string &filename);

	GLuint CompileShader(GLenum shaderType, const std::string &source);

	GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader, GLuint tcsShader, GLuint tesShader);
	GLuint LinkProgramPoints(GLuint vertexShader, GLuint fragmentShader);

	GLuint InitializeShaders();
	GLuint InitializePointsShader();

}



#endif /* SHADERTOOLS_H_ */
