/*
 * Program.cpp
 *
 *  Created on: Sep 10, 2018
 *      Author: John Hall
 */

#include "Program.h"

#include <iostream>
#include <string>
#include <time.h>

//**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "RenderingEngine.h"
#include "Scene.h"

Scene* currentScene;
float shift = 0.0f;
float speed = 0.07f;

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

	currentScene = scene;

	clock_t startTime = clock();

	//Main render loop
	while(!glfwWindowShouldClose(window)) {
		scene->displayScene();
		glfwSwapBuffers(window);
		glfwPollEvents();

		clock_t endTime = clock();
		Program* program = (Program*)glfwGetWindowUserPointer(window);

		std::cout << "Clocks " << CLOCKS_PER_SEC << std::endl;
		double elapsed = (double)(endTime - startTime) / CLOCKS_PER_SEC;

		if (elapsed >= 0.02) {
			startTime = clock();
			shift += -speed;
			//std::cout << "Current shift " << shift << std::endl;
			program->getScene()->scroller(shift);
		}
	}

}

Scene* Program::getScene() {
	return currentScene;
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

	//Set the custom function that tracks key presses
	glfwSetKeyCallback(window, KeyCallback);

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
	//Key codes are often prefixed with GLFW_KEY_ and can be found on the GLFW website
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	Program* program = (Program*)glfwGetWindowUserPointer(window);

	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		//std::cout << "1 PRESSED " << std::endl;
		program->getScene()->quadraticCurves();
		shift = 0.0f;
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		program->getScene()->cubicCurves();
		shift = 0.0f;
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		program->getScene()->myName();
		shift = 0.0f;
	}
	if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
		program->getScene()->scrollingText();
		shift = 0.0f;
	}

	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		program->getScene()->upArrow();
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		program->getScene()->downArrow();
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		if (speed <= 0.20f) {
			speed += 0.01f;
		}
		
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		if (speed > 0.02f) {
			speed -= 0.01f;
		}
	}
}
