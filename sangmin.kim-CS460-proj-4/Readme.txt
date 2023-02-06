CS460 Project #4

Ryan Kim

--------
OVERVIEW
--------
This project demonstrates project#4, physically-based-simulation
-----------
DESCRIPTION
-----------
Model - 900+ mass points and interaction with other objects

Model 75 * 75 (5625) mass points and * 4 springs (22500)
Mass points rendered with GL_POINTS, springs rendered with GL_LINES

----------
Instruction on compiling running the executable
----------
Compiling - run .sln file to open visual studio, build and run.

---------
INTERFACE
---------

I included Imgui for user interaction

Box tree
	- Contains sliders for change interaction object's size / position

AnchorPositions
	- Contains sliders for change anchors positions

Keyboard Controls
'Space'		: Toggle Camera movement / rotation.
'W/A/S/D'	: Camera movement.
'Mouse'		: Camera rotation.