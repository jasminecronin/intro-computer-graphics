/*
 * Scene.h
 *	Class for storing objects in a scene
 *  Created on: Sep 10, 2018
 *      Author: cb-ha
 * 
 * 	Modified by Jasmine Roebuck, 30037334
 *  Sep 25, 2018
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include <string>
#include <math.h>

#include "Geometry.h"

//Forward declaration of classes
//(note this is necessary because these are pointers and it allows the #include to appear in the .cpp file)
class RenderingEngine;

class Scene {

public:

	int level;
	void incLevel();
	void decLevel();

	Scene(RenderingEngine* renderer);
	virtual ~Scene();

	//Send geometry to the renderer
	void displayScene();

	void changeToDiamondScene();
	void drawSquare(float x1, float x2, float x3, float x4, float y1, float y2, float y3, float y4, int i);
	void changeToSpiralScene();
	void drawSpiral();
	void changeToTriangleScene();
	void drawTriangle(float x1, float x2, float x3, float y1, float y2, float y3, float red, float green, float blue, int i);
	void changeToSpongeScene();
	void changeToIterTriangleScene();
	void changeToFernScene();
	void changeToHilbertScene();
	void HilbertA(float x, float y, float length, int i, Geometry& path);
	void HilbertB(float x, float y, float length, int i, Geometry& path);
	void HilbertC(float x, float y, float length, int i, Geometry& path);
	void HilbertD(float x, float y, float length, int i, Geometry& path);

private:
	RenderingEngine* renderer;
	std::string scene;

	//list of objects in the scene
	std::vector<Geometry> objects;
};

#endif /* SCENE_H_ */
