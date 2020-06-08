/*
* Scene.cpp
*
*  Created on: Sep 10, 2018
*  Author: John Hall
*/

#include "Scene.h"

#include <iostream>

#include "RenderingEngine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GlyphExtractor.h"

int level = 0;
int currentScene = 0;
float stringLength;

Scene::Scene(RenderingEngine* renderer) : renderer(renderer) {
	quadraticCurves();
}

Scene::~Scene() {

}

void Scene::displayScene() {
	renderer->RenderScene(objects);
}

void Scene::resetScene() {
	objects.clear();
	bezier.verts.clear();
	bezier.colors.clear();
	points.verts.clear();
	points.colors.clear();
	borders.verts.clear();
	borders.colors.clear();
}

// Function takes in 2 points, an index i, and a degree of a curve n
// Returns a point inbetween pi and pj that is on the degree elevated curve
glm::vec3 Scene::elevate(glm::vec3 pi, glm::vec3 pj, int i, int n) {
	glm::vec3 q = ((i / (n + 1.0f)) * pi) + ((1 - (i / (n + 1.0f))) * pj);
	return q;

}

glm::mat3 Scene::transformMatrix(float scale, float angle, float translateX, float translateY) {
	glm::mat3 t;
	glm::mat3 r;

	// Scaling matrix, column major order
	glm::mat3 s = glm::mat3(scale, 0.0f, 0.0f,
							0.0f, scale, 0.0f,
							0.0f, 0.0f, 1.0f);

	// Rotation matrix, column major order
	r = glm::mat3(cos(angle), sin(angle), 0.0f,
					  -sin(angle), cos(angle), 0.0f,
					  0.0f, 0.0f, 1.0f);

	// Translation matrix, column major order
	t = glm::mat3(1.0f, 0.0f, 0.0f,
				  0.0f, 1.0f, 0.0f,
				  translateX, translateY, 1.0f);

	//Get transformation matrix and send it to the shader
	glm::mat3 transform = s * r * t;
	return transform;
}

void Scene::upArrow() { // Increment filter intensity
	//std::cout << "UP ARROW PRESSED "<< std::endl;
	if (level >= 0 && level < 2) {
		level++;
		//std::cout << "LEVEL INCREMENTED TO " << level << std::endl;
	}
	if (currentScene == 2) {
		myName();
		//std::cout << "SCENE REDRAWN " << std::endl;
	}
	if (currentScene == 3) {
		scrollingText();
	}
}

void Scene::downArrow() { // Decrement filter intesity
	if (level > 0 && level <= 2) {
		level--;
	}
	if (currentScene == 2) {
		myName();
	}
	if (currentScene == 3) {
		scrollingText();
	}
}

//Each of these scene changer functions needs to:
// add points and colors to the control points collection
// add points and colors to the borders collection
// draw each of these 3 objects
void Scene::quadraticCurves() {

	resetScene();
	currentScene = 0;

	glm::vec3 curves[12];
	curves[0] = glm::vec3(1.0f, 1.0f, 1.0f);
	curves[1] = glm::vec3(2.0f, -1.0f, 1.0f);
	curves[2] = glm::vec3(0.0f, -1.0f, 1.0f);

	curves[3] = glm::vec3(0.0f, -1.0f, 1.0f);
	curves[4] = glm::vec3(-2.0f, -1.0f, 1.0f);
	curves[5] = glm::vec3(-1.0f, 1.0f, 1.0f);

	curves[6] = glm::vec3(-1.0f, 1.0f, 1.0f);
	curves[7] = glm::vec3(0.0f, 1.0f, 1.0f);
	curves[8] = glm::vec3(1.0f, 1.0f, 1.0f);

	curves[9] = glm::vec3(1.2f, 0.5f, 1.0f);
	curves[10] = glm::vec3(2.5f, 1.0f, 1.0f);
	curves[11] = glm::vec3(1.3f, -0.4f, 1.0f);

	for (int i = 0; i < 12; i += 3) {
		bezier.verts.push_back(curves[i]);
		points.verts.push_back(curves[i]);
		points.colors.push_back(glm::vec3(1.0f, 0.2f, 0.0f));
		borders.verts.push_back(curves[i]);
		borders.colors.push_back(glm::vec3(0.4f, 0.4f, 0.4f));

		glm::vec3 q1 = elevate(curves[i], curves[i + 1], 1, 2);
		bezier.verts.push_back(q1);
		points.verts.push_back(curves[i + 1]);
		points.colors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
		borders.verts.push_back(curves[i + 1]);
		borders.colors.push_back(glm::vec3(0.4f, 0.4f, 0.4f));
		glm::vec3 q2 = elevate(curves[i + 1], curves[i + 2], 2, 2);
		bezier.verts.push_back(q2);

		bezier.verts.push_back(curves[i + 2]);
		points.verts.push_back(curves[i + 2]);
		points.colors.push_back(glm::vec3(1.0f, 0.2f, 0.0f));
		borders.verts.push_back(curves[i + 2]);
		borders.colors.push_back(glm::vec3(0.4f, 0.4f, 0.4f));

		// borders draw mode should be patches, will have to define that behavior
		// Patches of 2 probably will suffice
		// adding the points to points and borders: done as beziers are created, or do all in separate function?
		// can bezier.verts be accessed by index?
		// this will end up with a display of all cubic beziers. if we need to display different degrees,
		// adding the points will probably have to be done in each of these functions
	}

	for (int i = 0; i < bezier.verts.size(); i++) {
		bezier.colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	}

	bezier.drawMode = GL_PATCHES;
	points.drawMode = GL_POINTS;
	borders.drawMode = GL_LINE_STRIP;

	//Construct vao and vbos for the triangle
	RenderingEngine::assignBuffers(bezier);
	RenderingEngine::assignBuffers(points);
	RenderingEngine::assignBuffers(borders);

	//Send the triangle data to the GPU
	//Must be done every time the triangle is modified in any way, ex. verts, colors, normals, uvs, etc.
	RenderingEngine::setBufferData(bezier);
	RenderingEngine::setBufferData(points);
	RenderingEngine::setBufferData(borders);

	//Add the triangle to the scene objects
	objects.push_back(borders);
	objects.push_back(bezier);
	objects.push_back(points);

	glm::mat3 matrix = transformMatrix((1 / 2.6f), 0.0f, 0.0f, 0.0f);
	glUseProgram(renderer->shaderProgram);
	GLuint mat = glGetUniformLocation(renderer->shaderProgram, "transform");
	glUniformMatrix3fv(mat, 1, GL_FALSE, &matrix[0][0]);
	glUseProgram(renderer->pointsShader);
	mat = glGetUniformLocation(renderer->pointsShader, "transform");
	glUniformMatrix3fv(mat, 1, GL_FALSE, &matrix[0][0]);

	// then figure out how to draw the control points separately
	// and draw the control point outline separately
	// because we don't want the tess shaders to process those, only vertex and fragment

}

void Scene::cubicCurves() {

	resetScene();
	currentScene = 1;

	glm::vec3 curves[20];

	curves[0] = glm::vec3(1.0f, 1.0f, 1.0f);
	curves[1] = glm::vec3(4.0f, 0.0f, 1.0f);
	curves[2] = glm::vec3(6.0f, 2.0f, 1.0f);
	curves[3] = glm::vec3(9.0f, 1.0f, 1.0f);

	curves[4] = glm::vec3(8.0f, 2.0f, 1.0f);
	curves[5] = glm::vec3(0.0f, 8.0f, 1.0f);
	curves[6] = glm::vec3(0.0f, -2.0f, 1.0f);
	curves[7] = glm::vec3(8.0f, 4.0f, 1.0f);

	curves[8] = glm::vec3(5.0f, 3.0f, 1.0f);
	curves[9] = glm::vec3(3.0f, 2.0f, 1.0f);
	curves[10] = glm::vec3(3.0f, 3.0f, 1.0f);
	curves[11] = glm::vec3(5.0f, 2.0f, 1.0f);

	curves[12] = glm::vec3(3.0f, 2.2f, 1.0f);
	curves[13] = glm::vec3(3.5f, 2.7f, 1.0f);
	curves[14] = glm::vec3(3.5f, 3.3f, 1.0f);
	curves[15] = glm::vec3(3.0f, 3.8f, 1.0f);

	curves[16] = glm::vec3(2.8f, 3.5f, 1.0f);
	curves[17] = glm::vec3(2.4f, 3.8f, 1.0f);
	curves[18] = glm::vec3(2.4f, 3.2f, 1.0f);
	curves[19] = glm::vec3(2.8f, 3.5f, 1.0f);

	for (int i = 0; i < 20; i++) {
		bezier.verts.push_back(curves[i]);
		bezier.colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
		points.verts.push_back(curves[i]);
		borders.verts.push_back(curves[i]);
		borders.colors.push_back(glm::vec3(0.4f, 0.4f, 0.4f));
	}

	for (int i = 0; i < 5; i++) {
		points.colors.push_back(glm::vec3(1.0f, 0.2f, 0.0f));
		points.colors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
		points.colors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
		points.colors.push_back(glm::vec3(1.0f, 0.2f, 0.0f));
	}

	bezier.drawMode = GL_PATCHES;
	points.drawMode = GL_POINTS;
	borders.drawMode = GL_LINE_STRIP;

	//Construct vao and vbos for the triangle
	RenderingEngine::assignBuffers(bezier);
	RenderingEngine::assignBuffers(points);
	RenderingEngine::assignBuffers(borders);

	//Send the triangle data to the GPU
	//Must be done every time the triangle is modified in any way, ex. verts, colors, normals, uvs, etc.
	RenderingEngine::setBufferData(bezier);
	RenderingEngine::setBufferData(points);
	RenderingEngine::setBufferData(borders);

	//Add the triangle to the scene objects
	objects.push_back(borders);
	objects.push_back(bezier);
	objects.push_back(points);

	glm::mat3 matrix = transformMatrix((1 / 6.0f), 0.0f, -4.5f, -2.5f);
	glUseProgram(renderer->shaderProgram);
	GLuint mat = glGetUniformLocation(renderer->shaderProgram, "transform");
	glUniformMatrix3fv(mat, 1, GL_FALSE, &matrix[0][0]);
	glUseProgram(renderer->pointsShader);
	mat = glGetUniformLocation(renderer->pointsShader, "transform");
	glUniformMatrix3fv(mat, 1, GL_FALSE, &matrix[0][0]);

}

void Scene::myName() {

	currentScene = 2;
	resetScene();

	GlyphExtractor g;
	glm::mat3 matrix;

	if (level == 0) {
		g.LoadFontFile("fonts/lora/Lora-Regular.ttf");
		matrix = transformMatrix(0.4f, 0.0f, -1.7f, 0.0f);
	}
	else if (level == 1) {
		g.LoadFontFile("fonts/source-sans-pro/SourceSansPro-Regular.otf");
		matrix = transformMatrix(0.4f, 0.0f, -1.7f, 0.0f);
	}
	else if (level == 2) {
		g.LoadFontFile("fonts/great-vibes/GreatVibes-Regular.otf");
		matrix = transformMatrix(0.6f, 0.0f, -1.4f, 0.0f);
	}
	
	std::string myName = "Jasmine";
	float shift = 0.0f;
	for (int c = 0; c < myName.length(); c++) {
		MyGlyph character = g.ExtractGlyph(myName[c]);
		glm::mat3 shiftMatrix = transformMatrix(1.0f, 0.0f, shift, 0.0f);
		for (unsigned int i = 0; i < character.contours.size(); i++) {
			// A contour is a single continuous set of curves
			for (unsigned int j = 0; j < character.contours[i].size(); j++) {
				// a Bezier is a set of 2-4 control points
				if (character.contours[i][j].degree == 3) {
					for (unsigned int k = 0; k <= 3; k++) {
						bezier.verts.push_back(shiftMatrix * glm::vec3(character.contours[i][j].x[k], character.contours[i][j].y[k], 1.0f));
						bezier.colors.push_back(glm::vec3(1.0f, 0.25f, 0.0f));
					}
				}
				else if (character.contours[i][j].degree == 2) {
					glm::vec3 p0 = shiftMatrix * glm::vec3(character.contours[i][j].x[0], character.contours[i][j].y[0], 1.0f);
					glm::vec3 p1 = shiftMatrix * glm::vec3(character.contours[i][j].x[1], character.contours[i][j].y[1], 1.0f);
					glm::vec3 p2 = shiftMatrix * glm::vec3(character.contours[i][j].x[2], character.contours[i][j].y[2], 1.0f);
					// Add first point
					bezier.verts.push_back(p0);
					// Degree elevate and add 2nd point
					glm::vec3 q1 = elevate(p0, p1, 1, 2);
					bezier.verts.push_back(q1);
					// Degree elevate and add 3rd point
					glm::vec3 q2 = elevate(p1, p2, 2, 2);
					bezier.verts.push_back(q2);
					// Add last point
					bezier.verts.push_back(p2);
					for (int i = 0; i < 4; i++) {
						bezier.colors.push_back(glm::vec3(0.75f, 0.5f, 0.0f));
					}
				}
				else if (character.contours[i][j].degree == 1) {
					glm::vec3 p0 = shiftMatrix * glm::vec3(character.contours[i][j].x[0], character.contours[i][j].y[0], 1.0f);
					glm::vec3 p1 = shiftMatrix * glm::vec3(character.contours[i][j].x[1], character.contours[i][j].y[1], 1.0f);
					glm::vec3 mid = elevate(p0, p1, 1, 1);
					// Add first point
					bezier.verts.push_back(p0);
					// Degree elevate and add 2nd point
					glm::vec3 q1 = elevate(p0, mid, 1, 2);
					bezier.verts.push_back(q1);
					// Degree elevate and add 3rd point
					glm::vec3 q2 = elevate(mid, p1, 2, 2);
					bezier.verts.push_back(q2);
					// Add last point
					bezier.verts.push_back(p1);
					for (int i = 0; i < 4; i++) {
						bezier.colors.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
					}
				}
			}
		}
		shift += character.advance;
	}

	bezier.drawMode = GL_PATCHES;

	//Construct vao and vbos for the triangle
	RenderingEngine::assignBuffers(bezier);

	//Send the triangle data to the GPU
	//Must be done every time the triangle is modified in any way, ex. verts, colors, normals, uvs, etc.
	RenderingEngine::setBufferData(bezier);

	//Add the triangle to the scene objects
	objects.push_back(bezier);

	glUseProgram(renderer->shaderProgram);
	GLuint mat = glGetUniformLocation(renderer->shaderProgram, "transform");
	glUniformMatrix3fv(mat, 1, GL_FALSE, &matrix[0][0]);

}

void Scene::scrollingText() {

	currentScene = 3;
	resetScene();

	GlyphExtractor g;
	glm::mat3 matrix;

	if (level == 0) {
		g.LoadFontFile("fonts/lora/Lora-Regular.ttf");
		matrix = transformMatrix(0.4f, 0.0f, 2.5f, 0.0f);
	}
	else if (level == 1) {
		g.LoadFontFile("fonts/source-sans-pro/SourceSansPro-Regular.otf");
		matrix = transformMatrix(0.4f, 0.0f, 2.5f, 0.0f);
	}
	else if (level == 2) {
		g.LoadFontFile("fonts/great-vibes/GreatVibes-Regular.otf");
		matrix = transformMatrix(0.6f, 0.0f, 2.5f, 0.0f);
	}

	std::string sentence = "Jinxed wizards pluck ivy from the big quilt.";
	float shift = 0.0f;
	for (int c = 0; c < sentence.length(); c++) {
		MyGlyph character = g.ExtractGlyph(sentence[c]);
		glm::mat3 shiftMatrix = transformMatrix(1.0f, 0.0f, shift, 0.0f);
		for (unsigned int i = 0; i < character.contours.size(); i++) {
			// A contour is a single continuous set of curves
			for (unsigned int j = 0; j < character.contours[i].size(); j++) {
				// a Bezier is a set of 2-4 control points
				if (character.contours[i][j].degree == 3) {
					for (unsigned int k = 0; k <= 3; k++) {
						bezier.verts.push_back(shiftMatrix * glm::vec3(character.contours[i][j].x[k], character.contours[i][j].y[k], 1.0f));
						bezier.colors.push_back(glm::vec3(1.0f, 0.25f, 0.0f));
					}
				}
				else if (character.contours[i][j].degree == 2) {
					glm::vec3 p0 = shiftMatrix * glm::vec3(character.contours[i][j].x[0], character.contours[i][j].y[0], 1.0f);
					glm::vec3 p1 = shiftMatrix * glm::vec3(character.contours[i][j].x[1], character.contours[i][j].y[1], 1.0f);
					glm::vec3 p2 = shiftMatrix * glm::vec3(character.contours[i][j].x[2], character.contours[i][j].y[2], 1.0f);
					// Add first point
					bezier.verts.push_back(p0);
					// Degree elevate and add 2nd point
					glm::vec3 q1 = elevate(p0, p1, 1, 2);
					bezier.verts.push_back(q1);
					// Degree elevate and add 3rd point
					glm::vec3 q2 = elevate(p1, p2, 2, 2);
					bezier.verts.push_back(q2);
					// Add last point
					bezier.verts.push_back(p2);
					for (int i = 0; i < 4; i++) {
						bezier.colors.push_back(glm::vec3(0.75f, 0.5f, 0.0f));
					}
				}
				else if (character.contours[i][j].degree == 1) {
					glm::vec3 p0 = shiftMatrix * glm::vec3(character.contours[i][j].x[0], character.contours[i][j].y[0], 1.0f);
					glm::vec3 p1 = shiftMatrix * glm::vec3(character.contours[i][j].x[1], character.contours[i][j].y[1], 1.0f);
					glm::vec3 mid = elevate(p0, p1, 1, 1);
					// Add first point
					bezier.verts.push_back(p0);
					// Degree elevate and add 2nd point
					glm::vec3 q1 = elevate(p0, mid, 1, 2);
					bezier.verts.push_back(q1);
					// Degree elevate and add 3rd point
					glm::vec3 q2 = elevate(mid, p1, 2, 2);
					bezier.verts.push_back(q2);
					// Add last point
					bezier.verts.push_back(p1);
					for (int i = 0; i < 4; i++) {
						bezier.colors.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
					}
				}
			}
		}
		shift += character.advance;
	}

	stringLength = shift + 6;
	//std::cout << "String length: " << stringLength << std::endl;

	bezier.drawMode = GL_PATCHES;

	//Construct vao and vbos for the triangle
	RenderingEngine::assignBuffers(bezier);

	//Send the triangle data to the GPU
	//Must be done every time the triangle is modified in any way, ex. verts, colors, normals, uvs, etc.
	RenderingEngine::setBufferData(bezier);

	//Add the triangle to the scene objects
	objects.push_back(bezier);

	glUseProgram(renderer->shaderProgram);
	GLuint mat = glGetUniformLocation(renderer->shaderProgram, "transform");
	glUniformMatrix3fv(mat, 1, GL_FALSE, &matrix[0][0]);
}

void Scene::scroller(float shift) {

	if (currentScene == 3) {

		glm::mat3 matrix;
		shift = fmod(shift, stringLength);

		if (level == 0) {
			matrix = transformMatrix(0.4f, 0.0f, shift + 2.5f, 0.0f);
		}
		else if (level == 1) {
			matrix = transformMatrix(0.4f, 0.0f, shift + 2.5f, 0.0f);
		}
		else if (level == 2) {
			matrix = transformMatrix(0.6f, 0.0f, shift + 2.5f, 0.0f);
		}
		
		glUseProgram(renderer->shaderProgram);
		GLuint mat = glGetUniformLocation(renderer->shaderProgram, "transform");
		glUniformMatrix3fv(mat, 1, GL_FALSE, &matrix[0][0]);
	}
	
}



