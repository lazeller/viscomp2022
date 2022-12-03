#include "Helpers/GLShader.h"
#include <math.h>
#include "MainProgram.h"

#pragma pack(push, 1)

// A vertex containing only a position value
struct Vertex
{
	Vertex() {}

	Vertex(float x, float y, float z)
		: x(x), y(y), z(z) {}

	float x, y, z;
};

#pragma pack(pop)


MainProgram::MainProgram()
{
	m_pShader = NULL;

	m_vertexbuffer = 0;
	m_numVertices = 0;

	m_time = 0.0f;
}

MainProgram::~MainProgram()
{
	Shutdown();
}

bool MainProgram::Initialize()
{
	// Create and load the shader:
	m_pShader = new GLShader();
#if defined(__APPLE__)
    if (!m_pShader->loadFromFile(MAIN_DIR "OpenGL2.0/shaders/ex2.glslv", MAIN_DIR "OpenGL2.0/shaders/ex2.glslf"))
#else
	if (!m_pShader->loadFromFile(MAIN_DIR "shaders/ex2.glslv", MAIN_DIR "shaders/ex2.glslf"))
#endif
		return false;

	// Create geometry:
	glGenBuffers(1, &m_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);

	// Number of squares in x and y direction.
	// Each square contains two triangles.
	const int n = 64;

	m_numVertices = n*n*2*3;

	Vertex *pVertices = new Vertex[m_numVertices];

	for(int i=0; i<n; i++)
	{
		float x0 = (float)i / (float)n * 2.0f - 1.0f;
		float x1 = (float)(i+1) / (float)n * 2.0f - 1.0f;

		for(int j=0; j<n; j++)
		{
			float y0 = (float)j / (float)n * 2.0f - 1.0f;
			float y1 = (float)(j+1) / (float)n * 2.0f - 1.0f;

			// (x0,y1) ----- (x1,y1)
			//    |          /  |
			//    |        /    |
			//    |      /      |
			//    |    /        |
			//    |  /          |
			// (x0,y0) ----- (x1,y0)

			pVertices[(i*n+j)*2*3 + 0] = Vertex(x0, y0, 0);
			pVertices[(i*n+j)*2*3 + 1] = Vertex(x1, y0, 0);
			pVertices[(i*n+j)*2*3 + 2] = Vertex(x1, y1, 0);

			pVertices[(i*n+j)*2*3 + 3] = Vertex(x0, y0, 0);
			pVertices[(i*n+j)*2*3 + 4] = Vertex(x1, y1, 0);
			pVertices[(i*n+j)*2*3 + 5] = Vertex(x0, y1, 0);
		}
	}

	glBufferData(GL_ARRAY_BUFFER, m_numVertices*sizeof(Vertex), pVertices, GL_STATIC_DRAW);

	delete[] pVertices;

	return true;
}

void MainProgram::Shutdown()
{
	// Delete the shader:
	if(m_pShader != NULL)
	{
		delete m_pShader;
		m_pShader = NULL;
	}

	// Delete the vertex buffer:
	if(m_vertexbuffer != 0)
	{
		glDeleteBuffers(1, &m_vertexbuffer);
		m_vertexbuffer = 0;
	}
}

void MainProgram::Display(float deltaTime)
{
	m_time += deltaTime;

	// Clear the screen and depth-buffer:
	glClearColor(0.1f, 0.1f, 0.4f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable depth test:
	glEnable(GL_DEPTH_TEST);

	// EXERCISE 3:
	// Enable blending, so the final color is computed as:
	// final.rgb = src.rgb * src.a + dest.rgb * (1.0 - src.a)

	glCheckandPrintError();

	// EXERCISE 2
	// send current time to shader

	// Draw objects:
	float posX = 0.0; 
	float posY = 0.0; 
	float posZ = 0.0;

	// EXERCISE 2
	// calculate translation
	
	// draw first object
	renderObject(posX, posY, posZ);

	// EXERCISE 3:
	// Translate and draw second object:

	// Swap front and back buffer:
	glutSwapBuffers();

	// Disable the shader:
	m_pShader->unuseAll();
}

void MainProgram::renderScreenQuad()
{
	GLint location = m_pShader->getAttributeLocation("pos");
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(location);

	glDrawArrays(GL_TRIANGLES, 0, m_numVertices);

	glCheckandPrintError();
}

void MainProgram::renderObject(float posX, float posY, float posZ)
{
	// Set shader uniforms:
	float centerX = cos(m_time * 0.3f) * 0.4f;
	float centerY = sin(m_time * 0.3f + 0.4f) * 0.6f;
	m_pShader->setUniform(m_pShader->getUniformLocation("center"), centerX, centerY);

	float zoom = cos(m_time * 0.2f) * 0.4f + 0.6f;
	m_pShader->setUniform(m_pShader->getUniformLocation("zoom"), zoom);

	// EXERCISE 2:
	// set posX, posY and posZ

	// Activate the shader:
	m_pShader->use();

	glCheckandPrintError();

	// Draw geometry:
	renderScreenQuad();
}
