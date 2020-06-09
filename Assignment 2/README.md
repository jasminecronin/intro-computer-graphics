# Assignment 2

## Problems

![cap](https://github.com/jasminecronin/intro-computer-graphics/blob/master/Assignment%202/images/cap1.png)

![cap](https://github.com/jasminecronin/intro-computer-graphics/blob/master/Assignment%202/images/cap2.png)

![cap](https://github.com/jasminecronin/intro-computer-graphics/blob/master/Assignment%202/images/cap3.png)

![cap](https://github.com/jasminecronin/intro-computer-graphics/blob/master/Assignment%202/images/cap4.png)

![cap](https://github.com/jasminecronin/intro-computer-graphics/blob/master/Assignment%202/images/cap5.png)

![cap](https://github.com/jasminecronin/intro-computer-graphics/blob/master/Assignment%202/images/cap6.png)

![cap](https://github.com/jasminecronin/intro-computer-graphics/blob/master/Assignment%202/images/cap7.png)

## Solutions

![cap](https://github.com/jasminecronin/intro-computer-graphics/blob/master/Assignment%202/images/sol1.png)

![cap](https://github.com/jasminecronin/intro-computer-graphics/blob/master/Assignment%202/images/sol2.png)

![cap](https://github.com/jasminecronin/intro-computer-graphics/blob/master/Assignment%202/images/sol3.png)

## Running the code

Known Unresolved Issues:
	1. 	Translation only performs as expected prior to any rotating or scaling.
		It does not apply correctly if a translate is done after a rotation or a zoom.
	2. 	The U of C logo in the alpha compositing does not show up at the correct size.
	3.	There is no special logic implemented to handle edge pixels for the Sobel and Gaussian filters.
	4. 	Bonus was attempted but not completed.

Program was compiled in Visual Studio 2017 on Windows 10 by loading the .sln file. 

User Controls:

Number Keys 1-7 to change the image that is displayed:

	1 - Mandrill
	2 - Aerial view
	3 - Thirsk
	4 - Pattern
	5 - Bubble
	6 - U of C Logo with 3 backgrounds
	7 - Custom image - Sleeping Fox

Mouse Controls to transform the image:

	Scroll - Zoom in and out
	Left Mouse click & drag - Pan the image
	Right Mouse click & drag - Rotate the image

Arrow Keys to choose filters and intensity:

	Left & Right Arrow - Set filter type from 1-4
		1 - No filter
		2 - Color conversion effects
		3 - Edge effects
		4 - Blur effects
	Up & Down Arrow - Set intensity of filter from 1-3 (1-4 in the case of color conversion)
		Note that the up & down arrow keys are also used to change the background
		image in the U of C Logo scene. The above filters also apply to the foreground logo image, 
		but the background image and filter intensity cannot be set separately.
		i.e. Wood background = intensity 1, marble = intensity 2, asphalt = intensity 3
		
## Demo

Below is an example of the image filters applied. Top left: original image. Top right: greyscale conversion. Bottom left: Gaussian blur. Bottom right: Sobel edge filter.

![cap](https://github.com/jasminecronin/intro-computer-graphics/blob/master/Assignment%202/images/foxes.png)
