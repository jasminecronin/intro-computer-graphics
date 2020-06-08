/*
* Scene.cpp
*
*  Created on: Sep 10, 2018
*  Author: John Hall
* Edited by Jasmine Roebuck and Eddy Qiang
* December 11, 2018
*/

#include "Scene.h"

#include "RenderingEngine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

//**Must include glad and GLFW in this order or it breaks**
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "texture.h"

Scene::Scene(RenderingEngine* renderer, GLFWwindow* window) : renderer(renderer), window(window) {

	// Initialize spherical camera parameters
	r = 17;
	theta = 45;
	phi = 0;

	// Get sphere data, this adds 5 spheres to the object list
	parseFile("../Assets/sphere.obj");

	// Initialize all textures
	InitializeTexture(&objects.at(4).texture, "../Assets/Stars.png", GL_TEXTURE_2D);
	InitializeTexture(&objects.at(3).texture, "../Assets/mars.png", GL_TEXTURE_2D);
	InitializeTexture(&objects.at(2).texture, "../Assets/moon.png", GL_TEXTURE_2D);
	InitializeTexture(&objects.at(1).texture, "../Assets/earth.png", GL_TEXTURE_2D);
	InitializeTexture(&objects.at(0).texture, "../Assets/sun.png", GL_TEXTURE_2D);

	// Initialize positions
	objects.at(1).translate(4, 0, 0); //earth
	objects.at(2).translate(4.8, 0, 0); //moon
	objects.at(3).translate(6.2, 0, 0); //mars

	// Initialize scales
	objects.at(0).scale(2.0f); //sun
	objects.at(1).scale(0.5f); //earth
	objects.at(2).scale(0.15f); //moon
	objects.at(3).scale(0.4f); //mars
	objects.at(4).scale(50.0f); //stars

	// Turn off shading for sun and stars
	objects.at(0).shadingFlag = 0; //sun
	objects.at(4).shadingFlag = 0; //stars

	// Set rotation angles per frame
	objects.at(0).angle = 2 * 3.14f / 26.6f / 60.0f; //sun
	objects.at(1).angle = 2 * 3.14f / 60.0f; //earth
	objects.at(2).angle = 2 * 3.14f / 60.0f; //moon
	//objects.at(2).angle = 2 * 3.14f / 27.0f / 60.0f; //moon
	objects.at(3).angle = 2 * 3.14f / 1.03f / 60.0f; //mars
	objects.at(4).angle = 0.001; //stars
}

Scene::~Scene() {

}

void Scene::displayScene() {
	renderer->RenderScene(objects, window, theta, phi, r);
}

// Reads data from a .obj file and creates sphere objects
void Scene::parseFile(std::string filename) {

	Geometry sphere;

	// temporary lists
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempUVs;
	std::vector<glm::vec3> tempNormals;

	ifstream file;
	file.open(filename);
	if (!file) {
		cout << "Error in opening file" << endl;
	}

	string line;
	while(!file.eof())
	{
		getline(file, line);

		// Split each line into a list delimited by spaces
		std::vector<string> words;
		std::stringstream ss(line);
		std::string item;
		while (std::getline(ss, item, ' ')) {
			*(std::back_inserter(words)++) = item;
		}

		// If vertex line, get vertex positions
		if (words[0] == "v") { 
			tempVertices.push_back(glm::vec3(stof(words[1]), stof(words[2]), stof(words[3])));
		}
		// If texture line, get uv coords
		else if (words[0] == "vt") {
			tempUVs.push_back(glm::vec2(stof(words[1]), stof(words[2])));
		}
		// If normal line, get normals
		else if (words[0] == "vn") {
			tempNormals.push_back(glm::vec3(stof(words[1]), stof(words[2]), stof(words[3])));
		}
		// If faces line
		else if (words[0] == "f") {

			for (int i = 1; i < 4; i++) { // The remaining three 'words'

				// Same as above, but splits each 'word' into another list delimited by slashes
				std::vector<string> data;
				std::stringstream ss(&words[i].at(0));
				std::string num;
				while (std::getline(ss, num, '/')) {
					*(std::back_inserter(data)++) = num;
				}

				// Turn each of those numbers into indices for the temp lists
				int vert = atoi(&data[0].at(0)) - 1;
				int uv = atoi(&data[1].at(0)) - 1;
				int norm = atoi(&data[2].at(0)) - 1; 

				// Create the sphere
				sphere.verts.push_back(tempVertices.at(vert));
				sphere.uvs.push_back(tempUVs.at(uv));
				sphere.normals.push_back(tempNormals.at(norm));
			}
		}
	}

	file.close();

	sphere.drawMode = GL_TRIANGLES;
	RenderingEngine::assignBuffers(sphere);
	RenderingEngine::setBufferData(sphere);

	// Push 5 copies of the sphere onto the list
	objects.push_back(sphere);
	objects.push_back(sphere);
	objects.push_back(sphere);
	objects.push_back(sphere);
	objects.push_back(sphere);
}