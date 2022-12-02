#ifndef MAIN_PROGRAM_H
#define MAIN_PROGRAM_H

class GLShader;

class MainProgram
{
public:
	MainProgram();

	virtual ~MainProgram();

	// Initializes the program, returns true iff succeeded
	bool Initialize();

	// Shuts down the program, releases all OpenGL resources
	void Shutdown();

	// Renders the program. deltaTime is the time since the last call to Display(), in seconds
	void Display(float deltaTime);

private:
	// Renders a quad filling the whole screen (coordinates from [-1,-1,0] to [1,1,0])
	void renderScreenQuad();

	// Renders the object at position (posX, posY, posZ)
	void renderObject(float posX, float posY, float posZ);

private:
	// Shader to use for rendering
	GLShader *m_pShader;

	// Vertex buffer containing the object to render
	GLuint m_vertexbuffer;

	// Number of vertices in m_vertexbuffer
	int m_numVertices;

	// Current time in seconds
	float m_time;
};

#endif
