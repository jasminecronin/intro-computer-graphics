/*
* Program.cpp
*
*  Created on: Sep 10, 2018
*      Author: John Hall
*
*	Modified by Jasmine Roebuck
		October 10, 2018
*/

#include "Program.h"

#include <iostream>
#include <string>

//**Must include glad and GLFW in this order or it breaks**
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "RenderingEngine.h"
#include "Scene.h"
#include "texture.h"

//Bools to track if a mouse button is being held down
bool lButton = false;
bool rButton = false;

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
	scene = new Scene(renderingEngine);
	renderingEngine->scene = scene; //Give access to the texture member variables
	Program* program = (Program*)glfwGetWindowUserPointer(window);
	double xcoord, ycoord;

	//Main render loop
	while (!glfwWindowShouldClose(window)) {
		scene->displayScene();
		glfwSwapBuffers(window);
		glfwPollEvents();

		if (lButton == true) { //Track movement for translations
			glfwGetCursorPos(window, &xcoord, &ycoord);
			//std::cout << xcoord << std::endl;
			program->getScene()->setMouseMove(xcoord, ycoord);
		}
		else if (rButton == true) { //Track movement for rotations
			glfwGetCursorPos(window, &xcoord, &ycoord);
			//std::cout << xcoord << std::endl;
			program->getScene()->setCurrentRotate(xcoord, ycoord);
		}
		program->getScene()->transformationMatrix(lButton, rButton);
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
	int width = 1500;
	int height = 1500;
	window = glfwCreateWindow(width, height, "CPSC 453 OpenGL Boilerplate", 0, 0);
	if (!window) {
		std::cout << "Program failed to create GLFW window, TERMINATING" << std::endl;
		glfwTerminate();
		return;
	}

	glfwSetWindowUserPointer(window, this);
	//Set the custom function that tracks key presses
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);;
	//Set the custom function that tracks mouse scrolling
	glfwSetScrollCallback(window, ScrollCallback);

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

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	Program* program = (Program*)glfwGetWindowUserPointer(window);

	// User changes scene (which image to draw) with numbers 1-7
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		program->getScene()->displayMandrill();
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		program->getScene()->displayAerial();
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		program->getScene()->displayThirsk();
	}
	if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
		program->getScene()->displayPattern();
	}
	if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
		program->getScene()->displayBubble();
	}
	if (key == GLFW_KEY_6 && action == GLFW_PRESS) {
		program->getScene()->displayBackground();
	}
	if (key == GLFW_KEY_7 && action == GLFW_PRESS) {
		program->getScene()->displayFox();
	}

	// User changes intensity of filter with up and down arrow keys
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		program->getScene()->upArrow();
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		program->getScene()->downArrow();
	}
	// User changes type of filter with left and right arrow keys
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		program->getScene()->leftArrow();
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		program->getScene()->rightArrow();
	}
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

	double xcoord, ycoord;

	Program* program = (Program*)glfwGetWindowUserPointer(window);

	// Track movements for translation with left mouse button
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		lButton = true;
		glfwGetCursorPos(window, &xcoord, &ycoord);
		program->getScene()->setMouseDown(xcoord, ycoord);
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		lButton = false;
		glfwGetCursorPos(window, &xcoord, &ycoord);
		program->getScene()->setMouseUp(xcoord, ycoord);
	}

	// Track movements for rotation with right mouse button
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		rButton = true;
		glfwGetCursorPos(window, &xcoord, &ycoord);
		program->getScene()->setStartRotate(xcoord, ycoord);
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		rButton = false;
		glfwGetCursorPos(window, &xcoord, &ycoord);
		program->getScene()->setEndRotate(xcoord, ycoord);
	}
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {

	Program* program = (Program*)glfwGetWindowUserPointer(window);

	program->getScene()->setOffset(yoffset);
}
