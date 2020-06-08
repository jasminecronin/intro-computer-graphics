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

in vec2 uv;
in vec3 norm;
in vec3 vertex;

out vec4 FragmentColour;

uniform sampler2D imageTexture;
uniform vec3 eye;
uniform int flag;

void main(void)
{

	if (flag == 1) { // shading flag is on
	
		vec4 light = vec4(0, 0, 0, 1);
		vec4 ltemp = normalize(light - vec4(vertex, 1));
		vec3 l = vec3(ltemp.x, ltemp.y, ltemp.z); // light vector
		vec3 reflected = -normalize(l - (2 * dot(l, norm) * norm)); // reflected light vector
		vec3 view = normalize(eye - vertex); // view ray

		vec4 color = texture(imageTexture, uv); // get color from texture
		vec3 ambient = vec3(color.x, color.y, color.z) * 0.5;
		vec3 diffuse = vec3(0.64, 0.64, 0.64) * 0.5 * max(0, dot(norm, l));
		vec3 specular = vec3(0.5, 0.5, 0.5) * 0.5 * pow(max(0, dot(reflected, view)), 10);

		FragmentColour = vec4((ambient + diffuse + specular), 1.0);
	}
	else {
		FragmentColour = texture(imageTexture, uv);
	}
	
}
