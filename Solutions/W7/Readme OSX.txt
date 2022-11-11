================================================================================
====================    Visual Computing Exercises - OSX    ====================
================================================================================

Introduction
------------
The Visual Computing exercises use CMake to generate build files.


Requirements
------------
Make sure you have the following packages installed:

	- XCode or an alternative compiler that is supported by CMake
	- CMake
	- (optional) a GUI for CMake
	

Creating build files
--------------------
To create build files, run generateBuildOSX.sh. This will create a new folder
'build' that contains the build files. You can also change the compiler from
XCode to something else in the generateBuildOSX.sh file.
Alternatively, you can create the build files using a CMake GUI and place them
into a folder of your choosing.


Compiling and running your code
-------------------------------
Open the created XCode project and compile it.


Remark
------
If you run into any problems with OpenGL 3.0, you can switch to OpenGL 2.1 by
copying the files from the folder "OpenGL2.1" to your main directory. For this
you will also need to activate the branch for MacOS at the end of colors.cpp to
explicitly tell the OS to switch to this version. 