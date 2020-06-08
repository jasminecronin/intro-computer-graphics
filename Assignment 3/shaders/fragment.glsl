// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// interpolated colour received from vertex stage
in vec3 color;

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;

void main(void)
{
    // write colour output without modification
    FragmentColour = vec4(color, 1.0);
	//FragmentColour = texture(imageTexture, uv);
}
