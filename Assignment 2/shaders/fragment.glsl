// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// 
// Modified by Jasming Roebuck, 30037334
//		October 10, 2018
// ==========================================================================
#version 410

// interpolated colour received from vertex stage
in vec2 uv;

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;

// background texture is only used for compositing
uniform sampler2DRect imageTexture;
uniform sampler2DRect background;

// ints used to determine what filter effects to draw
uniform int effect;
uniform int level;
uniform int bg;

void main(void)
{
	// No image effect
	if (effect == 0) {
		FragmentColour = texture(imageTexture, uv);
	}


	// Color effects
	else if (effect == 1) {
		vec4 color = texture(imageTexture, uv);

		// Greyscale 1
		if (level == 0) { 
			float grey = (0.333 * color.x) + (0.333 * color.y) + (0.333 * color.z);
			FragmentColour = vec4(grey, grey, grey, color.a);
		}

		// Greyscale 2
		else if (level == 1) { 
			float grey = (0.299 * color.x) + (0.587 * color.y) + (0.114 * color.z);
			FragmentColour = vec4(grey, grey, grey, color.a);
		}

		// Greyscale 3
		else if (level == 2) { 
			float grey = (0.213 * color.x) + (0.715 * color.y) + (0.072 * color.z);
			FragmentColour = vec4(grey, grey, grey, color.a);
		}

		// Custom color conversion - inverted colors
		else if (level == 3) { 
			FragmentColour = vec4(1 - color.x, 1 - color.y, 1 - color.z, color.a);
		}
	}


	// Edge effects
	else if (effect == 2) { 
	
		float red = 0.0f;
		float green = 0.0f;
		float blue = 0.0f;

		// Make vec4 array of all surrounding pixel colors
		int n = 3;
		vec4 colors[9];
		for (int i = -(n/2); i <= (n/2); i++) {
			for (int j = -(n/2); j <= (n/2); j++) {
				colors[n * (i + (n/2)) + (j + (n/2))] = texture(imageTexture, vec2(uv[0] + i, uv[1] + j));
			}
		}

		// Vertical Sobel
		if (level == 0) { 
			red = colors[0].x + (2 * colors[1].x) + colors[2].x - colors[6].x - (2 * colors[7].x) - colors[8].x;
			green = colors[0].y + (2 * colors[1].y) + colors[2].y - colors[6].y - (2 * colors[7].y) - colors[8].y;
			blue = colors[0].z + (2 * colors[1].z) + colors[2].z - colors[6].z - (2 * colors[7].z) - colors[8].z;
		}

		// Horizontal Sobel
		else if (level == 1) { 
			red = -colors[0].x - (2 * colors[3].x) - colors[6].x + colors[2].x + (2 * colors[5].x) + colors[8].x;
			green = -colors[0].y - (2 * colors[3].y) - colors[6].y + colors[2].y + (2 * colors[5].y) + colors[8].y;
			blue = -colors[0].z - (2 * colors[3].z) - colors[6].z + colors[2].z + (2 * colors[5].z) + colors[8].z;
		}

		// Unsharp mask
		else if (level == 2 || level == 3) { 
			red = (5 * colors[4].x) - colors[1].x - colors[3].x - colors[5].x - colors[7].x;
			green = (5 * colors[4].y) - colors[1].y - colors[3].y - colors[5].y - colors[7].y;
			blue = (5 * colors[4].z) - colors[1].z - colors[3].z - colors[5].z - colors[7].z;
		}

		red = abs(red);
		green = abs(green);
		blue = abs(blue);
		FragmentColour = vec4(red, green, blue, texture(imageTexture, uv).a);
	}


	// Blur effects
	else if (effect == 3) { 

		float red = 0.0f;
		float green = 0.0f;
		float blue = 0.0f;

		// 3x3 filter
		if (level == 0) { 
			int n = 3;

			// Make matrix kernel
			vec4 colors[9];
			for (int i = -(n/2); i <= (n/2); i++) {
				for (int j = -(n/2); j <= (n/2); j++) {
					colors[n * (i + (n/2)) + (j + (n/2))] = texture(imageTexture, vec2(uv[0] + i, uv[1] + j));
				}
			}

			float gaussian[3] = { 0.2f, 0.6f, 0.2f };

			// Create color channels
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++) {
					red = red + (gaussian[i] * gaussian[j] * colors[n * i + j].x);
					green = green + (gaussian[i] * gaussian[j] * colors[n * i + j].y);
					blue = blue + (gaussian[i] * gaussian[j] * colors[n * i + j].z);
				}
			}
		}

		// 5x5 filter
		else if (level == 1) { 
			int n = 5;

			// Make matrix kernel
			vec4 colors[25];
			for (int i = -(n/2); i <= (n/2); i++) {
				for (int j = -(n/2); j <= (n/2); j++) {
					colors[n * (i + (n/2)) + (j + (n/2))] = texture(imageTexture, vec2(uv[0] + i, uv[1] + j));
				}
			}

			float gaussian[5] = { 0.06f, 0.24f, 0.4f, 0.24f, 0.06f };

			// Create color channels
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++) {
					red = red + (gaussian[i] * gaussian[j] * colors[n * i + j].x);
					green = green + (gaussian[i] * gaussian[j] * colors[n * i + j].y);
					blue = blue + (gaussian[i] * gaussian[j] * colors[n * i + j].z);
				}
			}
		}

		// 7x7 blur
		else if (level == 2 || level == 3) { 
			int n = 7;

			// Make matrix kernel
			vec4 colors[49];
			for (int i = -(n/2); i <= (n/2); i++) {
				for (int j = -(n/2); j <= (n/2); j++) {
					colors[n * (i + (n/2)) + (j + (n/2))] = texture(imageTexture, vec2(uv[0] + i, uv[1] + j));
				}
			}

			float gaussian[7] = { 0.03f, 0.11f, 0.22f, 0.28f, 0.22f, 0.11f, 0.03f };

			// Create color channels
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++) {
					red = red + (gaussian[i] * gaussian[j] * colors[n * i + j].x);
					green = green + (gaussian[i] * gaussian[j] * colors[n * i + j].y);
					blue = blue + (gaussian[i] * gaussian[j] * colors[n * i + j].z);
				}
			}
		}
		
		FragmentColour = vec4(red, green, blue, texture(imageTexture, uv).a);
	}


	// Bonus: Large Gaussian filter
	else if (effect == 4) { 

		float e = 2.71828;
		float pi = 3.14159;
		float red = 0.0f;
		float green = 0.0f;
		float blue = 0.0f;

		// 100x100 blur
		if (level == 0) {
			int n = 5;

			//Make matrix kernel
			vec4 horizontal[5];
			vec4 vertical[5];
			for (int i = -(n/2); i <= (n/2); i++) {
				horizontal[i + (n/2)] = texture(imageTexture, vec2(uv[0] + 1, uv[1]));
				vertical[i + (n/2)] = texture(imageTexture, vec2(uv[0], uv[1] + 1));
			}

			// Create Gaussian
			float gaussian[5];
			float sigma = float(n / 5.0f);
			float value;
			for (int i = (n/2); i >= 0; i--) {
				value = pow(e, (-(i * i)/(2 * sigma * sigma)))/sqrt(2 * pi * sigma * sigma);
				gaussian[(n - 1) - (n/2) - i] = value;
				value = pow(e, (-(i * i)/(2 * sigma * sigma)))/sqrt(2 * pi * sigma * sigma);
				gaussian[i + (n/2)] = value;
			}

			// Create color channels
			for (int i = 0; i < n; i++) {
				red = red + (gaussian[i] * horizontal[i].x) + (gaussian[i] * vertical[i].x);
				green = green + (gaussian[i] * horizontal[i].y) + (gaussian[i] * vertical[i].y);
				blue = blue + (gaussian[i] * horizontal[i].z) + (gaussian[i] * vertical[i].z);
			}

			FragmentColour = vec4(red, green, blue, texture(imageTexture, uv).a);
		}
	}


	// Alpha compositing
	if (bg == 1) { 
		mat2 scale = mat2(0.65f, 0.0f, 0.0f, 0.35);
		vec4 logo = texture(imageTexture, uv * scale);
		vec4 back = texture(background, uv);
		if (logo.a < 1) {
			FragmentColour = back;
		}
		else {FragmentColour = logo;}
	}


}


