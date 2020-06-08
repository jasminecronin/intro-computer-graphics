/*
* Scene.h
*	Class for storing objects in a scene
*  Created on: Sep 10, 2018
*      Author: cb-ha
*/

#ifndef SCENE_H_
#define SCENE_H_

#include <vector>

#include "Geometry.h"

//Forward declaration of classes
//(note this is necessary because these are pointers and it allows the #include to appear in the .cpp file)
class RenderingEngine;

class Scene {
public:
	Scene(RenderingEngine* renderer);
	virtual ~Scene();

	//Send geometry to the renderer
	void displayScene();
	void resetScene();
	glm::vec3 elevate(glm::vec3 pi, glm::vec3 pj, int i, int n);
	glm::mat3 transformMatrix(float scale, float rotate, float translateX, float translateY);
	void upArrow();
	void downArrow();
	void quadraticCurves();
	void cubicCurves();
	void myName();
	void scrollingText();
	void scroller(float shift);

private:
	RenderingEngine * renderer;

	Geometry bezier;
	Geometry points;
	Geometry borders;

	//list of objects in the scene
	std::vector<Geometry> objects;
};

#endif /* SCENE_H_ */
