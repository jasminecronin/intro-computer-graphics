Program was compiled on Windows 10 using Microsoft Visual Studio 2017.
Run the program from the solution file.

The three image renders are included in the 'Renders' folder.

To change scenes in the program, line 33 in 'Program.cpp' (This line is marked with @USER for ctrl+f) contains the scene call. Simply replace this line with one of the following:

For the first Cornell Box scene:
	scene->Scene1(); 
For the second icosahedron scene:
	scene->Scene2();
For the third custom scene:
	scene->Scene3();
 