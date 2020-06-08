// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
//
// Edited by: Jasmine Roebuck and Eddy Qiang
// Date: December 11, 2018
// ==========================================================================
#version 410

// location indices for these attributes correspond to those specified in the
// InitializeGeometry() function of the main program
layout(location = 0) in vec3 VertexPosition;
layout(location = 2) in vec3 VertexNormal; 
layout(location = 3) in vec2 VertexUV;

uniform mat4 V; // View matrix
uniform mat4 P; // Perspective matrix
uniform mat4 M; // Model matrix

out vec2 uv;
out vec3 norm;
out vec3 vertex;

void main()
{
	
    // assign vertex position after mutliplication with matrices
    gl_Position = P * V * M * vec4(VertexPosition, 1.0);

    // output texture coordinates
    uv = VertexUV;

	// Normalize normal vector, used for fragment shading
	vec4 temp = M * vec4(VertexNormal, 0.0f);
	norm = normalize(vec3(temp.x, temp.y, temp.z));

	// Position vector, used for fragment shading
	vec4 v = M * vec4(VertexPosition, 1.0);
	vertex = vec3(v.x, v.y, v.z);
}
