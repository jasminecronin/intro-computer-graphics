/*
* Scene.cpp
*
*  Created on: Sep 10, 2018
*  Author: John Hall
* 
*	Modified by Jasmine Roebuck, 30037334
*		October 10, 2018
*/

#include "Scene.h"

#include <iostream>

#include "RenderingEngine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//**Must include glad and GLFW in this order or it breaks**
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "texture.h"

int bg = 0; //Background mode flag
int effect = 0; //Type of effect filter
int level = 0; //Intensity of effect filter

Scene::Scene(RenderingEngine* renderer) : renderer(renderer) {
	// Enable alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	changeFilter(); // Set shader filters to default
	displayMandrill(); // Display the first image
}

Scene::~Scene() {}

// Clean up all the stuff when changing scenes
void Scene::resetScene() {
	// Clean up object and rectangle data
	objects.clear();
	rectangle.verts.clear();
	rectangle.uvs.clear();

	// Reset all transformation variables
	scaleFactor = 1.0f;
	setOffset(0.0f);
	shift.x = 0.0f;
	shift.y = 0.0f;
	startRotate = glm::vec3(0.0f, 0.0f, 1.0f);
	endRotate = glm::vec3(0.0f, 0.0f, 1.0f);
	spin = 0.0f;
	turn = 0.0f;

	bg = 0; // Background mode off
	// Removing this causes background images to remain on scene changes
	changeFilter();
}

// Redraw with different filter setting
void Scene::changeFilter() {
	// Pass new filter settings to the shader
	glUseProgram(renderer->shaderProgram);
	GLint int1 = glGetUniformLocation(renderer->shaderProgram, "effect");
	GLint int2 = glGetUniformLocation(renderer->shaderProgram, "level");
	GLint int3 = glGetUniformLocation(renderer->shaderProgram, "bg");
	glUniform1i(int1, effect);
	glUniform1i(int2, level);
	glUniform1i(int3, bg);
}

// User input functions
void Scene::leftArrow() { // Increment type of filter
	if (effect > 0 && effect <= 4) {
		effect--;
	}
	changeFilter();
	//std::cout << filter[0] << std::endl;
}

void Scene::rightArrow() { // Decrement type of filter
	if (effect >= 0 && effect < 4) {
		effect++;
	}
	changeFilter();
	//std::cout << filter[0] << std::endl;
}

void Scene::upArrow() { // Increment filter intensity
	if (level >= 0 && level < 3) {
		level++;
	}
	if (bg == 1) {
		displayBackground();
	}
	else {
		changeFilter();
	}
	//std::cout << filter[1] << std::endl;
}

void Scene::downArrow() { // Decrement filter intesity
	if (level > 0 && level <= 3) {
		level--;
	}
	if (bg == 1) {
		displayBackground();
	}
	else {
		changeFilter();
	}
	//std::cout << filter[1] << std::endl;
}

// Display functions
void Scene::displayScene() {
	renderer->RenderScene(objects);
}

void Scene::displayMandrill() {

	resetScene();
	MyTexture texture;
	InitializeTexture(&texture, "image1-mandrill.png", GL_TEXTURE_RECTANGLE);

	//Load texture uniform
	//Shaders need to be active to load uniforms
	glUseProgram(renderer->shaderProgram);
	//Set which texture unit the texture is bound to
	glActiveTexture(GL_TEXTURE0);
	//Bind the texture to GL_TEXTURE0
	glBindTexture(GL_TEXTURE_RECTANGLE, texture.textureID);
	//Get identifier for uniform
	GLuint uniformLocation = glGetUniformLocation(renderer->shaderProgram, "imageTexture");
	//Load texture unit number into uniform
	glUniform1i(uniformLocation, 0);

	if (renderer->CheckGLErrors()) {
		std::cout << "Texture creation failed" << std::endl;
	}

	//six vertex positions of 2 triangles that will form a square
	rectangle.verts.push_back(glm::vec3(-0.9f, -0.9f, 1.0f));
	rectangle.verts.push_back(glm::vec3(-0.9f, 0.9f, 1.0f));
	rectangle.verts.push_back(glm::vec3(0.9f, 0.9f, 1.0f));
	rectangle.verts.push_back(glm::vec3(-0.9f, -0.9f, 1.0f));
	rectangle.verts.push_back(glm::vec3(0.9f, 0.9f, 1.0f));
	rectangle.verts.push_back(glm::vec3(0.9f, -0.9f, 1.0f));

	rectangle.drawMode = GL_TRIANGLES;

	rectangle.uvs.push_back(glm::vec2(0.0f, 0.0f));
	rectangle.uvs.push_back(glm::vec2(0.0f, float(texture.height)));
	rectangle.uvs.push_back(glm::vec2(float(texture.width), float(texture.height)));
	rectangle.uvs.push_back(glm::vec2(0.0f, 0.0f));
	rectangle.uvs.push_back(glm::vec2(float(texture.width), float(texture.height)));
	rectangle.uvs.push_back(glm::vec2(float(texture.width), 0.0f));

	//Construct vao and vbos for the rectangle
	RenderingEngine::assignBuffers(rectangle);
	//Send the rectangle data to the GPU
	// Must be done every time the rectangle is modified in any way
	RenderingEngine::setBufferData(rectangle);
	//Add the rectangle to the scene objects
	objects.push_back(rectangle);
}

void Scene::displayAerial() {

	resetScene();
	MyTexture texture;
	InitializeTexture(&texture, "image3-aerial.jpg", GL_TEXTURE_RECTANGLE);

	//Load texture uniform
	//Shaders need to be active to load uniforms
	glUseProgram(renderer->shaderProgram);
	//Set which texture unit the texture is bound to
	glActiveTexture(GL_TEXTURE0);
	//Bind the texture to GL_TEXTURE0
	glBindTexture(GL_TEXTURE_RECTANGLE, texture.textureID);
	//Get identifier for uniform
	GLuint uniformLocation = glGetUniformLocation(renderer->shaderProgram, "imageTexture");
	//Load texture unit number into uniform
	glUniform1i(uniformLocation, 0);

	if (renderer->CheckGLErrors()) {
		std::cout << "Texture creation failed" << std::endl;
	}

	//six vertex positions of 2 triangles that will form a rectangle
	rectangle.verts.push_back(glm::vec3(-0.9f, -0.419f, 1.0f));
	rectangle.verts.push_back(glm::vec3(-0.9f, 0.419f, 1.0f));
	rectangle.verts.push_back(glm::vec3(0.9f, 0.419f, 1.0f));
	rectangle.verts.push_back(glm::vec3(-0.9f, -0.419f, 1.0f));
	rectangle.verts.push_back(glm::vec3(0.9f, 0.419f, 1.0f));
	rectangle.verts.push_back(glm::vec3(0.9f, -0.419f, 1.0f));

	rectangle.drawMode = GL_TRIANGLES;

	rectangle.uvs.push_back(glm::vec2(0.0f, 0.0f));
	rectangle.uvs.push_back(glm::vec2(0.0f, float(texture.height)));
	rectangle.uvs.push_back(glm::vec2(float(texture.width), float(texture.height)));
	rectangle.uvs.push_back(glm::vec2(0.0f, 0.0f));
	rectangle.uvs.push_back(glm::vec2(float(texture.width), float(texture.height)));
	rectangle.uvs.push_back(glm::vec2(float(texture.width), 0.0f));

	//Construct vao and vbos for the rectangle
	RenderingEngine::assignBuffers(rectangle);
	//Send the rectangle data to the GPU
	// Must be done every time the rectangle is modified in any way
	RenderingEngine::setBufferData(rectangle);
	//Add the rectangle to the scene objects
	objects.push_back(rectangle);
}

void Scene::displayThirsk() {

	resetScene();
	MyTexture texture;
	InitializeTexture(&texture, "image4-thirsk.jpg", GL_TEXTURE_RECTANGLE);

	//Load texture uniform
	//Shaders need to be active to load uniforms
	glUseProgram(renderer->shaderProgram);
	//Set which texture unit the texture is bound to
	glActiveTexture(GL_TEXTURE0);
	//Bind the texture to GL_TEXTURE0
	glBindTexture(GL_TEXTURE_RECTANGLE, texture.textureID);
	//Get identifier for uniform
	GLuint uniformLocation = glGetUniformLocation(renderer->shaderProgram, "imageTexture");
	//Load texture unit number into uniform
	glUniform1i(uniformLocation, 0);

	if (renderer->CheckGLErrors()) {
		std::cout << "Texture creation failed" << std::endl;
	}

	//six vertex positions of 2 triangles that will form a rectangle
	rectangle.verts.push_back(glm::vec3(-0.609f, -0.9f, 1.0f));
	rectangle.verts.push_back(glm::vec3(-0.609f, 0.9f, 1.0f));
	rectangle.verts.push_back(glm::vec3(0.609f, 0.9f, 1.0f));
	rectangle.verts.push_back(glm::vec3(-0.609f, -0.9f, 1.0f));
	rectangle.verts.push_back(glm::vec3(0.609f, 0.9f, 1.0f));
	rectangle.verts.push_back(glm::vec3(0.609f, -0.9f, 1.0f));

	rectangle.drawMode = GL_TRIANGLES;

	rectangle.uvs.push_back(glm::vec2(0.0f, 0.0f));
	rectangle.uvs.push_back(glm::vec2(0.0f, float(texture.height)));
	rectangle.uvs.push_back(glm::vec2(float(texture.width), float(texture.height)));
	rectangle.uvs.push_back(glm::vec2(0.0f, 0.0f));
	rectangle.uvs.push_back(glm::vec2(float(texture.width), float(texture.height)));
	rectangle.uvs.push_back(glm::vec2(float(texture.width), 0.0f));

	//Construct vao and vbos for the rectangle
	RenderingEngine::assignBuffers(rectangle);
	//Send the rectangle data to the GPU
	// Must be done every time the rectangle is modified in any way
	RenderingEngine::setBufferData(rectangle);
	//Add the rectangle to the scene objects
	objects.push_back(rectangle);
}

void Scene::displayPattern() {

	resetScene();
	MyTexture texture;
	InitializeTexture(&texture, "image5-pattern.png", GL_TEXTURE_RECTANGLE);

	//Load texture uniform
	//Shaders need to be active to load uniforms
	glUseProgram(renderer->shaderProgram);
	//Set which texture unit the texture is bound to
	glActiveTexture(GL_TEXTURE0);
	//Bind the texture to GL_TEXTURE0
	glBindTexture(GL_TEXTURE_RECTANGLE, texture.textureID);
	//Get identifier for uniform
	GLuint uniformLocation = glGetUniformLocation(renderer->shaderProgram, "imageTexture");
	//Load texture unit number into uniform
	glUniform1i(uniformLocation, 0);

	if (renderer->CheckGLErrors()) {
		std::cout << "Texture creation failed" << std::endl;
	}

	//six vertex positions of 2 triangles that will form a rectangle
	rectangle.verts.push_back(glm::vec3(-0.9f, -0.675f, 1.0f));
	rectangle.verts.push_back(glm::vec3(-0.9f, 0.675f, 1.0f));
	rectangle.verts.push_back(glm::vec3(0.9f, 0.675f, 1.0f));
	rectangle.verts.push_back(glm::vec3(-0.9f, -0.675f, 1.0f));
	rectangle.verts.push_back(glm::vec3(0.9f, 0.675f, 1.0f));
	rectangle.verts.push_back(glm::vec3(0.9f, -0.675f, 1.0f));

	rectangle.drawMode = GL_TRIANGLES;

	rectangle.uvs.push_back(glm::vec2(0.0f, 0.0f));
	rectangle.uvs.push_back(glm::vec2(0.0f, float(texture.height)));
	rectangle.uvs.push_back(glm::vec2(float(texture.width), float(texture.height)));
	rectangle.uvs.push_back(glm::vec2(0.0f, 0.0f));
	rectangle.uvs.push_back(glm::vec2(float(texture.width), float(texture.height)));
	rectangle.uvs.push_back(glm::vec2(float(texture.width), 0.0f));

	//Construct vao and vbos for the rectangle
	RenderingEngine::assignBuffers(rectangle);
	//Send the rectangle data to the GPU
	// Must be done every time the rectangle is modified in any way
	RenderingEngine::setBufferData(rectangle);
	//Add the rectangle to the scene objects
	objects.push_back(rectangle);
}

void Scene::displayBubble() {

	resetScene();
	MyTexture texture;
	InitializeTexture(&texture, "image6-bubble.png", GL_TEXTURE_RECTANGLE);

	//Load texture uniform
	//Shaders need to be active to load uniforms
	glUseProgram(renderer->shaderProgram);
	//Set which texture unit the texture is bound to
	glActiveTexture(GL_TEXTURE0);
	//Bind the texture to GL_TEXTURE0
	glBindTexture(GL_TEXTURE_RECTANGLE, texture.textureID);
	//Get identifier for uniform
	GLuint uniformLocation = glGetUniformLocation(renderer->shaderProgram, "imageTexture");
	//Load texture unit number into uniform
	glUniform1i(uniformLocation, 0);

	if (renderer->CheckGLErrors()) {
		std::cout << "Texture creation failed" << std::endl;
	}

	//six vertex positions of 2 triangles that will form a rectangle
	rectangle.verts.push_back(glm::vec3(-0.784f, -0.9f, 1.0f));
	rectangle.verts.push_back(glm::vec3(-0.784f, 0.9f, 1.0f));
	rectangle.verts.push_back(glm::vec3(0.784f, 0.9f, 1.0f));
	rectangle.verts.push_back(glm::vec3(-0.784f, -0.9f, 1.0f));
	rectangle.verts.push_back(glm::vec3(0.784f, 0.9f, 1.0f));
	rectangle.verts.push_back(glm::vec3(0.784f, -0.9f, 1.0f));

	rectangle.drawMode = GL_TRIANGLES;

	rectangle.uvs.push_back(glm::vec2(0.0f, 0.0f));
	rectangle.uvs.push_back(glm::vec2(0.0f, float(texture.height)));
	rectangle.uvs.push_back(glm::vec2(float(texture.width), float(texture.height)));
	rectangle.uvs.push_back(glm::vec2(0.0f, 0.0f));
	rectangle.uvs.push_back(glm::vec2(float(texture.width), float(texture.height)));
	rectangle.uvs.push_back(glm::vec2(float(texture.width), 0.0f));

	//Construct vao and vbos for the rectangle
	RenderingEngine::assignBuffers(rectangle);
	//Send the rectangle data to the GPU
	// Must be done every time the rectangle is modified in any way
	RenderingEngine::setBufferData(rectangle);
	//Add the rectangle to the scene objects
	objects.push_back(rectangle);
}

void Scene::displayBackground() {
	// Decide which background to display
	if (level == 0) {
		displayWood();
	}
	else if (level == 1) {
		displayMarble();
	} 
	else if (level == 2 || level == 3) {
		displayAsphalt();
	}
	changeFilter();
}

void Scene::displayWood() {

	resetScene();
	bg = 1; // Background mode on

	// Set the background texture
	InitializeTexture(&background, "background3-wood.jpg", GL_TEXTURE_RECTANGLE);
	glUseProgram(renderer->shaderProgram);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE, background.textureID);
	GLuint uniformLocation1 = glGetUniformLocation(renderer->shaderProgram, "background");
	glUniform1i(uniformLocation1, 2);

	//Set the foreground texture
	InitializeTexture(&foreground, "image2-uclogo.png", GL_TEXTURE_RECTANGLE);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE, foreground.textureID);
	GLuint uniformLocation2 = glGetUniformLocation(renderer->shaderProgram, "imageTexture");
	glUniform1i(uniformLocation2, 1);

	if (renderer->CheckGLErrors()) {
		std::cout << "Texture creation failed" << std::endl;
	}

	//six vertex positions of 2 triangles that will form a rectangle
	rectangle.verts.push_back(glm::vec3(-0.9f, -0.671f, 1.0f));
	rectangle.verts.push_back(glm::vec3(-0.9f, 0.671f, 1.0f));
	rectangle.verts.push_back(glm::vec3(0.9f, 0.671f, 1.0f));
	rectangle.verts.push_back(glm::vec3(-0.9f, -0.671f, 1.0f));
	rectangle.verts.push_back(glm::vec3(0.9f, 0.671f, 1.0f));
	rectangle.verts.push_back(glm::vec3(0.9f, -0.671f, 1.0f));

	rectangle.drawMode = GL_TRIANGLES;

	rectangle.uvs.push_back(glm::vec2(0.0f, 0.0f));
	rectangle.uvs.push_back(glm::vec2(0.0f, float(background.height)));
	rectangle.uvs.push_back(glm::vec2(float(background.width), float(background.height)));
	rectangle.uvs.push_back(glm::vec2(0.0f, 0.0f));
	rectangle.uvs.push_back(glm::vec2(float(background.width), float(background.height)));
	rectangle.uvs.push_back(glm::vec2(float(background.width), 0.0f));

	//Construct vao and vbos for the rectangle
	RenderingEngine::assignBuffers(rectangle);
	//Send the rectangle data to the GPU
	// Must be done every time the rectangle is modified in any way
	RenderingEngine::setBufferData(rectangle);
	//Add the rectangle to the scene objects
	objects.push_back(rectangle);
}

void Scene::displayMarble() {

	resetScene();
	bg = 1; // Background mode on

	// Set the background texture
	InitializeTexture(&background, "background2-marble.jpg", GL_TEXTURE_RECTANGLE);
	glUseProgram(renderer->shaderProgram);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE, background.textureID);
	GLuint uniformLocation = glGetUniformLocation(renderer->shaderProgram, "background");
	glUniform1i(uniformLocation, 2);

	// Set the foreground texture
	InitializeTexture(&foreground, "image2-uclogo.png", GL_TEXTURE_RECTANGLE);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE, foreground.textureID);
	GLuint uniformLocation2 = glGetUniformLocation(renderer->shaderProgram, "imageTexture");
	glUniform1i(uniformLocation2, 1);

	if (renderer->CheckGLErrors()) {
		std::cout << "Texture creation failed" << std::endl;
	}

	//six vertex positions of 2 triangles that will form a rectangle
	rectangle.verts.push_back(glm::vec3(-0.9f, -0.671f, 1.0f));
	rectangle.verts.push_back(glm::vec3(-0.9f, 0.671f, 1.0f));
	rectangle.verts.push_back(glm::vec3(0.9f, 0.671f, 1.0f));
	rectangle.verts.push_back(glm::vec3(-0.9f, -0.671f, 1.0f));
	rectangle.verts.push_back(glm::vec3(0.9f, 0.671f, 1.0f));
	rectangle.verts.push_back(glm::vec3(0.9f, -0.671f, 1.0f));

	rectangle.drawMode = GL_TRIANGLES;

	rectangle.uvs.push_back(glm::vec2(0.0f, 0.0f));
	rectangle.uvs.push_back(glm::vec2(0.0f, float(background.height)));
	rectangle.uvs.push_back(glm::vec2(float(background.width), float(background.height)));
	rectangle.uvs.push_back(glm::vec2(0.0f, 0.0f));
	rectangle.uvs.push_back(glm::vec2(float(background.width), float(background.height)));
	rectangle.uvs.push_back(glm::vec2(float(background.width), 0.0f));

	//Construct vao and vbos for the rectangle
	RenderingEngine::assignBuffers(rectangle);
	//Send the rectangle data to the GPU
	// Must be done every time the rectangle is modified in any way
	RenderingEngine::setBufferData(rectangle);
	//Add the rectangle to the scene objects
	objects.push_back(rectangle);
}

void Scene::displayAsphalt() {

	resetScene();
	bg = 1; // Background mode on

	// Set the background texture
	InitializeTexture(&background, "background1-asphalt.jpg", GL_TEXTURE_RECTANGLE);
	glUseProgram(renderer->shaderProgram);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE, background.textureID);
	GLuint uniformLocation = glGetUniformLocation(renderer->shaderProgram, "background");
	glUniform1i(uniformLocation, 2);

	// Set the foreground texture
	InitializeTexture(&foreground, "image2-uclogo.png", GL_TEXTURE_RECTANGLE);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE, foreground.textureID);
	GLuint uniformLocation2 = glGetUniformLocation(renderer->shaderProgram, "imageTexture");
	glUniform1i(uniformLocation2, 1);

	if (renderer->CheckGLErrors()) {
		std::cout << "Texture creation failed" << std::endl;
	}

	//six vertex positions of 2 triangles that will form a square
	rectangle.verts.push_back(glm::vec3(-0.9f, -0.671f, 1.0f));
	rectangle.verts.push_back(glm::vec3(-0.9f, 0.671f, 1.0f));
	rectangle.verts.push_back(glm::vec3(0.9f, 0.671f, 1.0f));
	rectangle.verts.push_back(glm::vec3(-0.9f, -0.671f, 1.0f));
	rectangle.verts.push_back(glm::vec3(0.9f, 0.671f, 1.0f));
	rectangle.verts.push_back(glm::vec3(0.9f, -0.671f, 1.0f));

	rectangle.drawMode = GL_TRIANGLES;

	rectangle.uvs.push_back(glm::vec2(0.0f, 0.0f));
	rectangle.uvs.push_back(glm::vec2(0.0f, float(background.height)));
	rectangle.uvs.push_back(glm::vec2(float(background.width), float(background.height)));
	rectangle.uvs.push_back(glm::vec2(0.0f, 0.0f));
	rectangle.uvs.push_back(glm::vec2(float(background.width), float(background.height)));
	rectangle.uvs.push_back(glm::vec2(float(background.width), 0.0f));

	//Construct vao and vbos for the rectangle
	RenderingEngine::assignBuffers(rectangle);
	//Send the rectangle data to the GPU
	// Must be done every time the rectangle is modified in any way
	RenderingEngine::setBufferData(rectangle);
	//Add the rectangle to the scene objects
	objects.push_back(rectangle);
}

void Scene::displayFox() {

	resetScene();
	MyTexture texture;
	InitializeTexture(&texture, "image7-fox.jpg", GL_TEXTURE_RECTANGLE);

	//Load texture uniform
	//Shaders need to be active to load uniforms
	glUseProgram(renderer->shaderProgram);
	//Set which texture unit the texture is bound to
	glActiveTexture(GL_TEXTURE0);
	//Bind the texture to GL_TEXTURE0
	glBindTexture(GL_TEXTURE_RECTANGLE, texture.textureID);
	//Get identifier for uniform
	GLuint uniformLocation = glGetUniformLocation(renderer->shaderProgram, "imageTexture");
	//Load texture unit number into uniform
	glUniform1i(uniformLocation, 0);

	if (renderer->CheckGLErrors()) {
		std::cout << "Texture creation failed" << std::endl;
	}

	//six vertex positions of 2 triangles that will form a rectangle
	rectangle.verts.push_back(glm::vec3(-0.9f, -0.6f, 1.0f));
	rectangle.verts.push_back(glm::vec3(-0.9f, 0.6f, 1.0f));
	rectangle.verts.push_back(glm::vec3(0.9f, 0.6f, 1.0f));
	rectangle.verts.push_back(glm::vec3(-0.9f, -0.6f, 1.0f));
	rectangle.verts.push_back(glm::vec3(0.9f, 0.6f, 1.0f));
	rectangle.verts.push_back(glm::vec3(0.9f, -0.6f, 1.0f));

	rectangle.drawMode = GL_TRIANGLES;

	rectangle.uvs.push_back(glm::vec2(0.0f, 0.0f));
	rectangle.uvs.push_back(glm::vec2(0.0f, float(texture.height)));
	rectangle.uvs.push_back(glm::vec2(float(texture.width), float(texture.height)));
	rectangle.uvs.push_back(glm::vec2(0.0f, 0.0f));
	rectangle.uvs.push_back(glm::vec2(float(texture.width), float(texture.height)));
	rectangle.uvs.push_back(glm::vec2(float(texture.width), 0.0f));

	//Construct vao and vbos for the rectangle
	RenderingEngine::assignBuffers(rectangle);
	//Send the rectangle data to the GPU
	// Must be done every time the rectangle is modified in any way
	RenderingEngine::setBufferData(rectangle);
	//Add the rectangle to the scene objects
	objects.push_back(rectangle);
}

// Transformation utility functions

// Scale
void Scene::setOffset(double offset) {
	// Set zoom factor with scroll wheel offset
	if ((scaleFactor + offset > 0.1) && scaleFactor + offset < 10) {
		scaleFactor = scaleFactor + offset;
	}
}

// Translation
void Scene::setMouseDown(double xval, double yval) {
	// Normalize the screen coordinates
	// These are the starting coordinates to determine the translation
	mouseDown.x = (xval / 750.f) - 1;
	mouseDown.y = -((yval / 750.f) + 1);
}

void Scene::setMouseUp(double xval, double yval) {
	// Normalize the screen coordinates
	// These are the coordinates to help track previous translations
	mouseUp.x = (xval / 750.f) - 1;
	mouseUp.y = -((yval / 750.f) + 1);
	shift = shift + (mouseUp - mouseDown);
}

void Scene::setMouseMove(double xval, double yval) {
	// Normalize the screen coordinates
	// These are the coordinates of the translation during mouse movement
	mouseMove.x = (xval / 750.0f) - 1;
	mouseMove.y = -((yval / 750.0f) + 1);
	move = shift + (mouseMove - mouseDown);
}

// Rotation
void Scene::setStartRotate(double xval, double yval) {
	// Normalize the vector in the window
	// This is the starting vector to determing degree of rotation
	startRotate.x = float(xval);
	startRotate.y = float(yval);
	startRotate = startRotate - glm::vec3(750.0f, 750.0f, 1.0f);
}

void Scene::setEndRotate(double xval, double yval) {
	// Normalize the vector in the window
	// This is the ending vector to help track previous rotations
	endRotate.x = float(xval);
	endRotate.y = float(yval);
	endRotate = endRotate - glm::vec3(750.0f, 750.0f, 1.0f);
	turn = spin;
}

void Scene::setCurrentRotate(double xval, double yval) {
	// Normalize the vector in the window
	// This determines the angle of rotation while the mouse is moving
	currentRotate.x = float(xval);
	currentRotate.y = float(yval);
	currentRotate = currentRotate - glm::vec3(750.0f, 750.0f, 1.0f);
	float angle1 = atan2(startRotate.y, startRotate.x);
	float angle2 = atan2(currentRotate.y, currentRotate.x);
	spin = turn + (angle1 - angle2);
}

// Build the transformation matrix
void Scene::transformationMatrix(bool lButton, bool rButton) {
	glm::mat3 translate;
	glm::mat3 rotate;

	// Scaling matrix, column major order
	glm::mat3 scale = glm::mat3(scaleFactor, 0.0f,		  0.0f, 
								0.0f,		 scaleFactor, 0.0f, 
								0.0f,		 0.0f,		  1.0f);

	// Rotation matrix, column major order
	if (rButton) {
		rotate = glm::mat3(cos(spin),  sin(spin), 0.0f,
						   -sin(spin), cos(spin), 0.0f,
						   0.0f,	   0.0f,	  1.0f);
	}
	else {
		rotate = glm::mat3(cos(turn),  sin(turn), 0.0f,
						   -sin(turn), cos(turn), 0.0f,
						   0.0f,	   0.0f,	  1.0f);
	}

	// Translation matrix, column major order
	if (lButton) {
		translate = glm::mat3(1.0f,   0.0f,   0.0f,
							  0.0f,   1.0f,   0.0f,
							  move.x, move.y, 1.0f);
	}
	else {
		translate = glm::mat3(1.0f,	   0.0f,    0.0f,
							  0.0f,    1.0f,    0.0f,
							  shift.x, shift.y, 1.0f);
	}
	//Get transformation matrix and send it to the shader
	glm::mat3 transform = scale * rotate * translate;
	glUseProgram(renderer->shaderProgram);
	GLuint matrix1 = glGetUniformLocation(renderer->shaderProgram, "transform");
	glUniformMatrix3fv(matrix1, 1, GL_FALSE, &transform[0][0]);
	//std::cout << transform[0][0] << std::endl;
}

