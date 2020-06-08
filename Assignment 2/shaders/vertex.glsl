// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
//
// Modified by Jasmine Roebuck, 30037334
//		October 10, 2018
// ==========================================================================
#version 410

// location indices for these attributes correspond to those specified in the
// InitializeGeometry() function of the main program
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexUV;

uniform mat3 transform; // Transformation matrix

// output to be interpolated between vertices and passed to the fragment stage
out vec2 uv;

void main()
{
	vec3 transformPosition = transform * VertexPosition;
	gl_Position = vec4(transformPosition.xy, 0.0, 1.0);

    // assign output colour to be interpolated
    uv = VertexUV;
}
