================================================================================
====================   Visual Computing Exercises - Linux   ====================
================================================================================

Introduction
------------
The Visual Computing exercises use CMake to generate build files.


Requirements
------------
Make sure you have the following packages installed:

	- GCC/G++
	- CMake
	- suitable graphics card drivers that support OpenGL 3 and higher, for
	  example Mesa
	- (optional) a GUI for CMake
	

Creating build files
--------------------
To create build files, run generateBuildLinux.sh. This will create a new folder
'build' that contains the build files for a debug and release build.
Alternatively, you can create the build files using a CMake GUI and place them
into a folder of your choosing.


Compiling and running your code
-------------------------------
Move into either the "build/Debug" or "build/Release" folder and run "make" to
compile your code. To run your code, execute the created executable (e.g., 
"./colors" for the first exercise.


Remarks
-------
If you run into any problems with OpenGL 3.0, you can switch to OpenGL 2.1 by
copying the files from the folder "OpenGL2.1" to your main directory. 