This is build instructions for course [252-0206-00L Visual Computing](https://cvg.ethz.ch/teaching/visualcomputing/) Exercise 9.

## Linux
To build and run the code base, run
```
mkdir build
cd build
cmake .. -DOpenGL_GL_PREFERENCE=GLVND
make
./shaders
```

## macOS
To build and run the code base, run
```
mkdir build
cd build
cmake .. 
make
./shaders
```
Please implement the exercise shader code the `ex2.glslf` and `ex2.glslv` inside the `OpenGL2.0/shaders` folder.


## Windows
To create build files, follow the steps below
1. Edit generateBuildWindows.bat and change the variable
CMAKE_GENERATOR to the compiler/IDE of your choice ("Visual Studio 12" would
be Visual Studio 2013).
2. Run `generateBuildWindows.bat`.

Alternatively, you can create the build files using a CMake GUI and place them
into a folder of your choosing.

To compile your code, in Visual Studio, open the generated Visual Studio solution in the `build`
folder and compile using `Build` -> `Build Solution` (or press `F7`). Then
right click your project in the solution explorer (e.g., "shaders" for this
exercise) and select `Set up as StartUp Project`. To run your code, click the
green arrow or press `F5`.



