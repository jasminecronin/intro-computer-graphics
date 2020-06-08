#version 410

layout(isolines) in;

in vec3 teColour[];

out vec3 color;

#define PI 3.14159265359

void main()
{
	float u = gl_TessCoord.x;

	vec3 startColour = teColour[0];
	vec3 endColour = teColour[1];

	vec2 p0 = gl_in[0].gl_Position.xy;
	vec2 p1 = gl_in[1].gl_Position.xy;
	vec2 p2 = gl_in[2].gl_Position.xy;
	vec2 p3 = gl_in[3].gl_Position.xy;
	
	float b0 = (1 - u) * (1 - u) * (1 - u);
	float b1 = 3 * (1 - u) * (1 - u) * u;
	float b2 = 3 * (1 - u) * u * u;
	float b3 = u * u * u;

	vec2 position = (b0 * p0) + (b1 * p1) + (b2 * p2) + (b3 * p3);

	gl_Position = vec4(position, 0, 1);

	color = endColour;

}
