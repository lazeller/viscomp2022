================================================================================
====================  Visual Computing Exercises - Windows  ====================
================================================================================

Introduction
------------
The Visual Computing exercises use CMake to generate build files.


Requirements
------------
Make sure you have the following programs installed:

	- Visual Studio (any version) or an alternative compiler that is supported
	  by CMake
	- CMake (tested with version 3.14)
	- suitable graphics card drivers that support OpenGL 3 and higher
	- (optional) a GUI for CMake
	

Creating build files
--------------------
To create build files, edit generateBuildWindows.bat and change the variable
CMAKE_GENERATOR to the compiler/IDE of your choice ("Visual Studio 16 2019" would
be Visual Studio 2019). Then run generateBuildWindows.bat. This will create a
new folder 'build' that contains the build files.
Alternatively, you can create the build files using a CMake GUI and place them
into a folder of your choosing.


Compiling and running your code
-------------------------------
For Visual Studio, open the generated Visual Studio solution in the 'build'
folder and compile using "Build" -> "Build Solution" (or press F7). Then
right-click your project in the solution explorer (e.g., "colors" for the first
exercise) and select "Set up as StartUp Project". To run your code, click the
green arrow or press F5.


Remark
------
If you run into any problems with OpenGL 3.0, you can switch to OpenGL 2.1 by
copying the files from the folder "OpenGL2.1" to your main directory. 