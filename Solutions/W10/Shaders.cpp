#include <stdlib.h>
#include "Helpers/opengl.h"
#include "MainProgram.h"

#ifdef _WIN32
#include <MMSystem.h>
#else
#include <sys/time.h>
#include <stdio.h>
#endif

using namespace std;

#ifdef _WIN32
class SystemTimer
{
public:
	SystemTimer() {
		timeBeginPeriod(1);
		m_dwTimeStart = timeGetTime();
	}

	virtual ~SystemTimer() {
		timeEndPeriod(1);
	}

	float GetTime() {
		return (timeGetTime() - m_dwTimeStart) / 1000.0f;
	}

private:
	DWORD m_dwTimeStart;
};
#else
class SystemTimer
{
public:
	SystemTimer() {
		m_timeStart = 0.0f;
		m_timeStart = GetTime();
	}
	
	float GetTime() {
		struct timeval tv;
		gettimeofday(&tv, 0);
		return (double)((tv.tv_sec)*1000000 + tv.tv_usec) * 1e-6 - m_timeStart;
	}
	
private:
	float m_timeStart;
};
#endif

static SystemTimer g_sysTimer;

static float g_timePrev = 0.0f;



static MainProgram *g_pMainProgram = NULL;


//=======================================================================
// Application initialization
//=======================================================================
bool initialize()
{
	g_pMainProgram = new MainProgram();
	return g_pMainProgram->Initialize();
}


//=======================================================================
// Application shutdown
//=======================================================================
void shutdown()
{
	if(g_pMainProgram != NULL)
	{
		g_pMainProgram->Shutdown();

		delete g_pMainProgram;
		g_pMainProgram = NULL;
	}
}


//=======================================================================
// GLUT display function callback
//=======================================================================
void displayFunc()
{
	float time = g_sysTimer.GetTime();
	float deltaTime = time - g_timePrev;
	g_timePrev = time;

	if(g_pMainProgram != NULL)
		g_pMainProgram->Display(deltaTime);

	// Display again as soon as possible:
	glutPostRedisplay();
}


//=======================================================================
// GLUT reshape function callback
//=======================================================================
void reshapeFunc(int w, int h)
{
	// Scale the image to the size of the window
	glViewport(0, 0, w, h);
}


//=======================================================================
// GLUT keyboard function callback
//=======================================================================
void keyboardFunc(unsigned char key, int x, int y)
{
	if(key == 27) // ESC
	{
		shutdown();

		// There is no nice way to exit from the glut main loop...
		exit(0);
	}
}


//=======================================================================
// Entry point
//=======================================================================
int main(int argc, char **argv)
{
	// Initialize the glut environment
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);

	// create a new window named 'Shaders'
	glutCreateWindow("Shaders");

	// Check required hardware support
#if defined(__APPLE__)
	if(!GLEE_VERSION_2_0)
    {
		cerr << "Need OpenGL 2.0 support" << endl;
#else 
    if (!GLEE_VERSION_3_0)
    {
		cerr << "Need OpenGL 3.0 support" << endl;
#endif
		getchar();
		return 1;
	}

	if(!initialize())
	{
		cerr << "Initialization failed" << endl;
		getchar();
		return 2;
	}

	// Set the size of the window
	glutReshapeWindow(800, 600);

	// Register the display callback function
	glutDisplayFunc(displayFunc);

	// Register the reshape callback function
	glutReshapeFunc(reshapeFunc);

	// Register keyboard callback function
	glutKeyboardFunc(keyboardFunc);

	// Hand over the control to GLUT server loop
	glutMainLoop();

	return 0;
}
