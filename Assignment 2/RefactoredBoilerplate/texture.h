#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// --------------------------------------------------------------------------
// Functions to set up OpenGL buffers for storing textures

struct MyTexture
{
	GLuint textureID;
	GLuint target;
	int width;
	int height;

	// initialize object names to zero (OpenGL reserved value)
	MyTexture();
};

bool InitializeTexture(MyTexture* texture, const char* filename, GLenum target = GL_TEXTURE_2D);

// deallocate texture-related objects
void DestroyTexture(MyTexture *texture);