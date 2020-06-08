/*
* Scene.h
*	Class for storing objects in a scene
*  Created on: Sep 10, 2018
*      Author: cb-ha
* Edited by Jasmine Roebuck and Eddy Qiang
* December 11, 2018
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
	Scene(RenderingEngine* renderer, GLFWwindow* window);
	virtual ~Scene();

	//Send geometry to the renderer
	void displayScene();

	// Parameters for spherical camera
	float theta;
	float phi;
	float r;

private:
	RenderingEngine * renderer;
	GLFWwindow* window;

	//list of objects in the scene
	std::vector<Geometry> objects;

	// Gets data from a .obj file
	void parseFile(std::string filename);

};

#endif /* SCENE_H_ */
