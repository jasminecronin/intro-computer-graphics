/*
* Program.cpp
*
*  Created on: Sep 10, 2018
*      Author: John Hall
* Edited by Jasmine Roebuck and Eddy Qiang
* December 11, 2018
*/

#include "Program.h"

#include <iostream>
#include <string>

//**Must include glad and GLFW in this order or it breaks**
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "RenderingEngine.h"
#include "Scene.h"

Program::Program() {
	setupWindow();
}

Program::~Program() {
	//Must be cleaned up in the destructor because these are allocated to the heap
	delete renderingEngine;
	delete scene;
}

void Program::start() {
	renderingEngine = new RenderingEngine();
	scene = new Scene(renderingEngine, window);

	//Main render loop
	while (!glfwWindowShouldClose(window)) {
		scene->displayScene();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

}

void Program::setupWindow() {
	//Initialize the GLFW windowing system
	if (!glfwInit()) {
		std::cout << "ERROR: GLFW failed to initialize, TERMINATING" << std::endl;
		return;
	}

	//Set the custom error callback function
	//Errors will be printed to the console
	glfwSetErrorCallback(ErrorCallback);

	//Attempt to create a window with an OpenGL 4.1 core profile context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	int width = 1024;
	int height = 1024;
	window = glfwCreateWindow(width, height, "CPSC 453 OpenGL Boilerplate", 0, 0);
	if (!window) {
		std::cout << "Program failed to create GLFW window, TERMINATING" << std::endl;
		glfwTerminate();
		return;
	}
	glfwSetWindowUserPointer(window, this);

	//Set the custom function that tracks key presses
	glfwSetKeyCallback(window, KeyCallback);

	//Set the custom function that tracks scroll wheel 
	glfwSetScrollCallback(window, ScrollCallback);

	//Set the custom function that tracks cursor position 
	glfwSetCursorPosCallback(window, cursor_position_callback);

	//Bring the new window to the foreground (not strictly necessary but convenient)
	glfwMakeContextCurrent(window);

	//Intialize GLAD (finds appropriate OpenGL configuration for your system)
	if (!gladLoadGL()) {
		std::cout << "GLAD init failed" << std::endl;
		return;
	}

	//Query and print out information about our OpenGL environment
	QueryGLVersion();
}



void Program::QueryGLVersion() {
	// query opengl version and renderer information
	std::string version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
	std::string glslver = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
	std::string renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));

	std::cout << "OpenGL [ " << version << " ] "
		<< "with GLSL [ " << glslver << " ] "
		<< "on renderer [ " << renderer << " ]" << std::endl;
}

void ErrorCallback(int error, const char* description) {
	std::cout << "GLFW ERROR " << error << ":" << std::endl;
	std::cout << description << std::endl;
}

// These are used to adjust the position of the spherical camera
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//Key codes are often prefixed with GLFW_KEY_ and can be found on the GLFW website
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	Program* p = (Program*)glfwGetWindowUserPointer(window);
	Scene* s = p->getScene();
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		s->theta -= 10;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		s->theta += 10;
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		s->phi += 5;
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		s->phi -= 5;
	}

	if (s->theta > 179.99) {
		s->theta = 179.99;
	}
	if (s->theta < 0.01) {
		s->theta = 0.01;
	}

	if (s->phi > 359.99) {
		s->phi = 359.99;
	}
	if (s->phi < 0.01) {
		s->phi = 0.01;
	}
}

// Used to zoom in and out of the orrery
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {

	Program* program = (Program*)glfwGetWindowUserPointer(window);

	if ((program->getScene()->r + yoffset > 1.5) && (program->getScene()->r + yoffset < 50)) {
		program->getScene()->r += yoffset;
	}
}

double rx; // x position of the mouse when it is clicked
double ry; // y position of the mouse when it is clicked

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Program* p = (Program*)glfwGetWindowUserPointer(window);
	Scene* s = p->getScene();

	/*
	//debug messages
	std::cout << "x: ";
	std::cout << xpos;
	std::cout << "     y: ";
	std::cout << ypos << std::endl;
	*/

	GLboolean leftDown = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) ? true : false;

	if (!(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS))
	{
		rx = xpos;
		ry = ypos;
	}
	if (leftDown)
	{
		//changes the phi and theta according to mouse offset
		s->phi += ((rx - xpos)/256);
		s->theta += ((ry - ypos)/128);

		// bounds limiting the rotation to 1 round
		if (s->theta > 179.99) {
			s->theta = 179.99;
		}
		if (s->theta < 0.01) {
			s->theta = 0.01;
		}
		if (s->phi > 359.99) {
			s->phi = 359.99;
		}
		if (s->phi < 0.01) {
			s->phi = 0.01;
		}
	}
}
