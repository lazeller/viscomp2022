#include <stdio.h>
#include <assert.h>
#include <fstream>
#include <string>
#include <iostream>
#include <math.h>
#include <GLee.h>
#include <GLFW/glfw3.h>

# if defined(__APPLE__) && defined(__MACH__)
#    include <OpenGL/glu.h>
# else
#    include <GL/glu.h>
# endif


using namespace std;

// Default name of the mesh file to load
#define DEFAULT_MESH_NAME MAIN_DIR "volcano_mesh.raw"

#ifndef offsetof
#define offsetof(s, m)   (size_t)&(((s *)0)->m)
#endif

#define GLCheckError()	do {	GLenum err = glGetError(); if(err != GL_NO_ERROR)											\
								cerr << "OPENGL_ERROR:" << __FILE__ << ":" << __LINE__ << ":" << gluErrorString(err) << " (" << err << ")" << endl;	\
						} while(0)

// A triangle has three associated vertices and a normal vector
struct Triangle
{
	// Vertex pIndices of the triangle
	int vtxIndexA;
	int vtxIndexB;
	int vtxIndexC;

	// Normal vector of the triangle
	GLfloat normal[3];
};

// A vertex has a position, a color and a normal vector
struct Vertex
{
	// Position of the vertex
	GLfloat pos[4];

	// R/G/B/Alpha color of the vertex
	GLfloat color[4];

	// Normal vector of the vertex
	GLfloat normal[3];
};


// Constants for coordinate axes
#define COR_X 0
#define COR_Y 1
#define COR_Z 2
#define COR_W 3

// Position of a point of interest
float g_interestingPoint[3] = { 200.0f, 200.0f, 90.0f };

// Handle of shader program which defines the material of our object
GLuint g_shaderProgram = 0;

// Handle of vertex buffer object containing position, color and normal for each vertex
GLuint g_modelVBO = 0;

// Handle of index buffer object (defines the triangles)
GLuint g_modelIBO = 0;

// Number of entries in g_modelIBO
GLuint g_modelIBOSize = 0;


//=======================================================================
// Converts hsv to rgb
//=======================================================================
// IN:  hsv color coordinates h in [0,360), s in [0,1], v in [0,1]
// OUT: rgb coordinates of hsv color, rgb in [0,1]^3
//=======================================================================
void hsv2rgb(float h, float s, float v, float* pR, float* pG, float* pB)
{
	assert(h >= 0.0f && h < 360.0f);
	assert(s >= 0.0f && s <= 1.0f);
	assert(v >= 0.0f && v <= 1.0f);

	// Get hue in [0,1)
	h /= 360.0;
	if (h == 1.0f)
		h = 0.0f;
	h *= 6.0f;

	// Get used segment i, i in [0,5]
	int i = (int)floor(h);

	// Get the fractional part of h
	float f = h - i;
	float p = v * (1 - s);
	float q = v * (1 - (s * f));
	float t = v * (1 - (s * (1 - f)));

	// Cases for every segment
	switch (i)
	{
	case 0: *pR = v; *pG = t; *pB = p; break;
	case 1: *pR = q; *pG = v; *pB = p; break;
	case 2: *pR = p; *pG = v; *pB = t; break;
	case 3: *pR = p; *pG = q; *pB = v; break;
	case 4: *pR = t; *pG = p; *pB = v; break;
	case 5: *pR = v; *pG = p; *pB = q; break;
	}
}


//=======================================================================
// Exercise 2b)
// 
// Calculates and defines the surface normals for all vertices by
// combining the normals of all adjacent triangles.
//=======================================================================
// IN:  pTriangleArray: List of triangles
//      numTriangles:   number of triangles in pTriangleArray
//      numVertices:    Number of triangles in pVertices
// OUT: pVertices:      The 'normal' member of each vertex is modified
//=======================================================================
void calcVertexNormals(const Triangle* pTriangleArray, int numTriangles, Vertex* pVertices, int numVertices)
{
	// Set all vertex normals to zero
	// [..code..]

	// For each vertex, sum up normals of adjacent triangles
	// [..code..]

	// Normalize all vertex normals	
	// [..code..]
}


//=======================================================================
// Exercise 3)
// 
// Defines, respectively calculates the color for the given vertex in
// dependence of its 3D coordinates
//=======================================================================
// IN:  pCoordinate: The 3D coordinates of the vertex
// OUT: pResult:     Color at that position
//=======================================================================
void calcVertexColor(float* pCoordinate, float* pResult)
{
	// color of a vertex given in the HSV space
	float h, s, v;
	// color of a vertex given in the RGB space
	float r, g, b;

	// The hue serves as color table for the height of a vertex
	// Z values lie between 37 and 126 and the color table starts with red and ends with blue
	// Hint: Set alpha channel (color component 4) to 1 (=solid)
	// [..code..]

	// default color to white, delete for Ex.3
	pResult[0] = 1.;
	pResult[1] = 1.;
	pResult[2] = 1.;
	pResult[3] = 1.0f;	// alpha channel (1 = solid)
}


//=======================================================================
// Loads an surface mesh from the given file and generates the
// corresponding display list.
//=======================================================================
// IN:  meshFileName: Filename of the file containing of a list of
//                    vertices and list of triangles
//=======================================================================
bool loadMeshObject(const string& meshFileName)
{
	//------------------------------------------------------------------
	// open the mesh file
	//------------------------------------------------------------------

	cout << "Mesh file: " << meshFileName;

	// Open the input mesh file
	ifstream inputFile;
	inputFile.open(meshFileName.c_str());

	// check if the file is correct opened */
	if (!inputFile)
	{
		cerr << " failed to open!" << endl;
		return false;
	}
	else
	{
		cout << " opened." << endl;
	}


	//------------------------------------------------------------------
	// read the data info
	//------------------------------------------------------------------

	// the first number gives us the data-format of the file
	// (we don't use this number here)
	int dataFormat;
	inputFile >> dataFormat;

	// get the number of vertices
	int numVertices;
	inputFile >> numVertices;

	// get the number of triangles
	int numTriangles;
	inputFile >> numTriangles;


	//------------------------------------------------------------------
	// read the vertices and store them in the vertex array
	//------------------------------------------------------------------

	// Create vertex array of size numVertices
	Vertex* pVertexArray = new Vertex[numVertices];

	// Read vertices from the input file
	for (int vtxIndex = 0; vtxIndex < numVertices; vtxIndex++)
	{
		inputFile >> pVertexArray[vtxIndex].pos[COR_X];
		inputFile >> pVertexArray[vtxIndex].pos[COR_Y];
		inputFile >> pVertexArray[vtxIndex].pos[COR_Z];
		pVertexArray[vtxIndex].pos[COR_W] = 1.0;
	}


	//------------------------------------------------------------------
	// read the triangles and store them in struct array
	//------------------------------------------------------------------

	Triangle* pTriangleArray = new Triangle[numTriangles];

	for (int triIndex = 0; triIndex < numTriangles; triIndex++)
	{
		// Read the vertex indices of the triangle from the input file
		inputFile >> pTriangleArray[triIndex].vtxIndexA;
		inputFile >> pTriangleArray[triIndex].vtxIndexB;
		inputFile >> pTriangleArray[triIndex].vtxIndexC;
	}


	//------------------------------------------------------------------
	// close the input file
	//------------------------------------------------------------------

	inputFile.close();


	//==================================================================
	// EXERCISE 2a)
	// calculate the triangle normals. Store them in the normal
	// component of pTriangleArray.
	//==================================================================

	// Traverse the triangle list
	for (int triIndex = 0; triIndex < numTriangles; triIndex++)
	{
		// Positions of the triangles nodes
		GLfloat positionA[3];
		GLfloat positionB[3];
		GLfloat positionC[3];

		// Get the positions of the triangles nodes
		// [..code..]

		// Vectors for the calculation of the normal
		GLfloat vec1[3];
		GLfloat vec2[3];

		// Calculate two difference vectors of the triangle
		// [..code..]

		// Calculate the normal vector: vector product between x and y
		// [..code..]

		// Normalize the normal vector:
		// [..code..]
	}

	// Calculate vertex normals
	calcVertexNormals(pTriangleArray, numTriangles, pVertexArray, numVertices);

	// Calculate vertex colors
	for (int i = 0; i < numVertices; i++)
	{
		calcVertexColor(pVertexArray[i].pos, pVertexArray[i].color);
	}

	// Create index array for triangles
	g_modelIBOSize = numTriangles * 3;
	GLshort* pIndices = new GLshort[g_modelIBOSize];
	for (int triIndex = 0; triIndex < numTriangles; triIndex++)
	{
		pIndices[3 * triIndex + 0] = pTriangleArray[triIndex].vtxIndexA;
		pIndices[3 * triIndex + 1] = pTriangleArray[triIndex].vtxIndexB;
		pIndices[3 * triIndex + 2] = pTriangleArray[triIndex].vtxIndexC;
	}

	//===============================================================
	// EXERCISE 1)
	// Setup vertex, index buffers and bind the attributes
	//===============================================================

	// Create vertex buffer object and download vertex array
	// [..code..]

	GLint location;
	// bind Vertex shader per-vertex attributes
	// [..code..]

	// Create index buffer and download index data
	// [..code..]


	delete[] pIndices;
	delete[] pVertexArray;
	delete[] pTriangleArray;

	GLCheckError();

	return true;
}


//=======================================================================
// Reads a text file
//=======================================================================
// IN:  Name of the file
// OUT: Contents of the file as a string
//=======================================================================
string readTextFile(const string& fileName)
{
	// open file
	ifstream ifs(fileName.c_str(), ios::binary);
	if (!ifs)
	{
		cerr << "File not found" << endl;
		return "";
	}

	// Determine length of file
	ifs.seekg(0, ios::end);
	size_t fileLen = ifs.tellg();
	ifs.seekg(0, ios::beg);

	// Allocate string buffer and read string from file
	char* pStr = new char[fileLen + 1];
	ifs.read(pStr, fileLen);
	pStr[fileLen] = 0;

	// Convert to STL string
	string s = pStr;
	delete[] pStr;

	return s;
}


//=======================================================================
// Creates two shader objects, fills them with the shader source,
// compiles them, creates a new Program object, attaches both shaders
// to this object, links the program object, returns its identifier.
//=======================================================================
// IN:  path to a GLSL vertex shader file, path to a GLSL fragment shader file
// OUT: GLuint identifier of the (Shader)Program object
//=======================================================================
GLuint createShaderObject(const string& vertex_file, const string& fragment_file)
{
	GLint status;

	// create both shader objects
	GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);

	// load source code from file an add to shader objects
	string sourceCode = readTextFile(vertex_file);
	if (sourceCode.length() == 0)
	{
		cerr << "create Shader Object failed";
		return 0;
	}
	const char* pS = sourceCode.c_str();
	glShaderSource(vshader, 1, &pS, NULL);

	sourceCode = readTextFile(fragment_file);
	if (sourceCode.length() == 0)
	{
		cerr << "create Shader Object failed";
		return 0;
	}
	pS = sourceCode.c_str();
	glShaderSource(fshader, 1, &pS, NULL);

	// compile both shaders
	glCompileShader(vshader);
	glGetShaderiv(vshader, GL_COMPILE_STATUS, &status);
	if (status == 0)
	{
		GLint length;
		glGetShaderiv(vshader, GL_INFO_LOG_LENGTH, &length);
		char* errorstr = new char[length];
		glGetShaderInfoLog(vshader, length, NULL, errorstr);
		cerr << vertex_file << " compile error(s):" << endl << errorstr;
		delete[] errorstr;
		return 0;
	}

	glCompileShader(fshader);
	glGetShaderiv(fshader, GL_COMPILE_STATUS, &status);
	if (status == 0)
	{
		GLint length;
		glGetShaderiv(fshader, GL_INFO_LOG_LENGTH, &length);
		char* errorstr = new char[length];
		glGetShaderInfoLog(fshader, length, NULL, errorstr);
		cerr << fragment_file << " compile error(s):" << endl << errorstr;
		delete[] errorstr;
		return 0;
	}

	// create Program object
	GLuint program = glCreateProgram();

	// attach both shaders
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);

	// Link Program
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == 0)
	{
		GLint length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		char* errorstr = new char[length];
		glGetProgramInfoLog(program, length, NULL, errorstr);
		cerr << vertex_file << " " << fragment_file << " link error(s):" << endl << errorstr;
		delete[] errorstr;
		return 0;
	}

	// Flag the shader objects for deletion so they get deleted
	// automatically when the program object is deleted.
	glDeleteShader(vshader);
	glDeleteShader(fshader);

	return program;
}


//=======================================================================
// Creates the shader program and sets some shader constants
//=======================================================================
bool initGraphics(void)
{
	// Create Shader Object
	g_shaderProgram = createShaderObject(MAIN_DIR "shaders/color.glslv", MAIN_DIR "shaders/color.glslf");

	if (g_shaderProgram == 0)
		return false;

	// Use that shader program for rendering
	glUseProgram(g_shaderProgram);

	// Set model matrix
	GLfloat model_mat[16] = {
		 0.008000f,  0.000000f,  0.000000f, 0.000000f,
		 0.000000f,  0.008000f,  0.000000f, 0.000000f,
		 0.000000f,  0.000000f,  0.008000f, 0.000000f,
		-1.300000f, -1.500000f, -1.000000f, 1.0000000f
	};

	GLint location = glGetUniformLocation(g_shaderProgram, "Model_mat");
	glUniformMatrix4fv(location, 1, false, model_mat);

	// Set projection*view Matrix
	GLfloat projview_mat[16] = {
		2.747478f, 0.000000f,  0.000000f,  0.000000f,
		0.000000f, 2.457419f,  0.546594f,  0.447214f,
		0.000000f, 1.228709f, -1.093189f, -0.894427f,
		0.000000f, 0.000000f,  1.877236f,  3.354102f
	};
	location = glGetUniformLocation(g_shaderProgram, "ProjectView_mat");
	glUniformMatrix4fv(location, 1, false, projview_mat);

	// Set position of light
	GLfloat lightpos[4] = { 500.0f, 200.0f, 300.0f, 1.0f };
	location = glGetUniformLocation(g_shaderProgram, "lightPos");
	glUniform4fv(location, 1, lightpos);

	// Preset some Attributes in case no array is set
	location = glGetAttribLocation(g_shaderProgram, "normal");
	assert(location != -1);
	glVertexAttrib4f(location, 0.0f, 1.0f, 0.0f, 1.0f);
	location = glGetAttribLocation(g_shaderProgram, "color_in");
	assert(location != -1);
	glVertexAttrib4f(location, 1.0f, 1.0f, 1.0f, 1.0f);

	return true;
}


//=======================================================================
// GLFW display function
//=======================================================================
void displayFunc(void)
{
	// clear the window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//===============================================================
	// EXERCISE 4) 
	// Set uniform of interesting point to g_InterestingPoint
	//===============================================================
	// [..code..]

	// Draw all triangles defined in the index buffer object
	// with the vertex data from vertex array in the vertex buffer object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_modelIBO);
	glDrawElements(GL_TRIANGLES, g_modelIBOSize, GL_UNSIGNED_SHORT, 0);

	GLCheckError();
}


//=======================================================================
// GLFW reshape function callback
//=======================================================================
void reshapeFunc(GLFWwindow* window, int w, int h)
{
	// Scale the image to the size of the window
	glViewport(0, 0, w, h);
}


//=======================================================================
// Program entry point
//=======================================================================
int main(int argc, char** argv)
{
	// Initialize the glfw environment
	if (!glfwInit()) return -1;

	// Create a new window named 'Colors'
	GLFWwindow* window = glfwCreateWindow(800, 600, "Colors", NULL, NULL);

	// Activate OpenGL 2.1 for MacOS
	if (false) {
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	}

	// Check the window initialization
	if (!window) {
		glfwTerminate();
		return -1;
	}

	// Set the window context
	glfwMakeContextCurrent(window);

	// Register the reshape callback function
	glfwSetFramebufferSizeCallback(window, reshapeFunc);

	// Initialize the OpenGL rendering
	if (!initGraphics())
		return 1;

	// Name of the mesh to load
	string meshFileName = DEFAULT_MESH_NAME;

	// Check if a argument is given, use argument as file name
	if (argc == 2)
		meshFileName = argv[1];

	// Load mesh file
	if (!loadMeshObject(meshFileName))
		return 1;

	// Run GLFW loop
	while (!glfwWindowShouldClose(window)) {

		displayFunc();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}
