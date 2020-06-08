/*
* Scene.h
*	Class for storing objects in a scene
*  Created on: Sep 10, 2018
*      Author: cb-ha
*
*	Modified by Jasmine Roebuck, 30037334
		October 10, 2018
*/

#ifndef SCENE_H_
#define SCENE_H_

#include <vector>

#include "Geometry.h"
#include "texture.h"

//Forward declaration of classes
//(note this is necessary because these are pointers and it allows the #include to appear in the .cpp file)
class RenderingEngine;

class Scene {
public:
	Scene(RenderingEngine* renderer);
	virtual ~Scene();

	//Send geometry to the renderer
	void displayScene();

	//Utility functions
	void resetScene();
	void leftArrow();
	void rightArrow();
	void upArrow();
	void downArrow();
	void changeFilter();

	//Display various scenes with different images
	void displayMandrill();
	void displayAerial();
	void displayThirsk();
	void displayPattern();
	void displayBubble();
	void displayBackground();
	void displayWood();
	void displayMarble();
	void displayAsphalt();
	void displayFox();

	//Functions to set transformation variables
	void setOffset(double offset);
	void setMouseDown(double xval, double yval);
	void setMouseUp(double xval, double yval);
	void setMouseMove(double xval, double yval);
	void setStartRotate(double xval, double yval);
	void setEndRotate(double xval, double yval);
	void setCurrentRotate(double xval, double yval);
	void transformationMatrix(bool lButton, bool rButton);

	//Textures for alpha compositing
	MyTexture background;
	MyTexture foreground;

private:
	RenderingEngine * renderer;

	Geometry rectangle;

	//Variables to track transformations
	float scaleFactor = 1.0f;
	glm::vec3 mouseDown = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 mouseUp = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 mouseMove = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 move = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 shift = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 startRotate = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 endRotate = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 currentRotate = glm::vec3(0.0f, 0.0f, 1.0f);
	float spin = 0.0f;
	float turn = 0.0f;

	//list of objects in the scene
	std::vector<Geometry> objects;
};

#endif /* SCENE_H_ */
