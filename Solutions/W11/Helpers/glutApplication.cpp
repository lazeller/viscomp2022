//to implement gametimer
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#include <stdlib.h>
#endif

#include <assert.h>
#include <list>
#include <iostream>
#include <map>
#include "glutApplication.h"

static std::list< int > g_windows;

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

//helpers
static float _loop_time = 0.0f;

static void _SetCurrentLoopTime() 
{
	_loop_time = g_sysTimer.GetTime();
}

static float _GetCurrentLoopTime() 
{
	return _loop_time;
}


std::map<int, void*> g_windowData;

void setWindowData(void *pData)
{
	int id = glutGetWindow();

	g_windowData[id] = pData;
}

void *getWindowData()
{
	int id = glutGetWindow();

	std::map<int, void*>::const_iterator iter = g_windowData.find(id);
	if(iter == g_windowData.end())
		return NULL;

	return iter->second;
}


//Callback functions
static void _glutIdleFunc()
{
	//update time of this loop
	_SetCurrentLoopTime();

	std::list< int >::const_iterator it = g_windows.begin();
	std::list< int >::const_iterator end = g_windows.end();
	float t = _GetCurrentLoopTime();
	
	if(g_windows.size() > 1)
		glFinish();   // seems to be needed. looks like a driver bug 

	for(; it != end; it++) {
		glutSetWindow(*it);
		GlutWindow *window = (GlutWindow *)getWindowData();
		window->Update( t );
	}	
}

static void _CloseFunc()
{
	int w = glutGetWindow();
	GlutWindow *window = (GlutWindow *)getWindowData();
	if(window == NULL)
		return;

	window->CloseEvent();
	window->SetDestroyed(true);

	g_windows.remove(w);
	if(g_windows.size() < 1)
	{
#ifndef NO_FREEGLUT
		glutLeaveMainLoop();
#else
		exit(0);
#endif
	}
}

static GlutWindow *getCurrentWindow()
{
	GlutWindow *window = (GlutWindow *)getWindowData();	
	assert(window);
	return window;
}

static void _KeyboardFunc(unsigned char key, int x, int y) 
{
	getCurrentWindow()->KeyEvent(key, GLUT_DOWN, glutGetModifiers(), x, y);
}

static void _SpecialFunc(int key, int x, int y)
{
	getCurrentWindow()->SpecialKeyEvent(key, GLUT_DOWN, glutGetModifiers(), x, y);
}

static void _ReshapeFunc(int x, int y)
{
	getCurrentWindow()->ReshapeEvent(x, y);
}

static void _DisplayFunc(void)
{	
	float time = _GetCurrentLoopTime();
	getCurrentWindow()->Draw(time);
	glutSwapBuffers();
}

static void _MouseFunc(int button, int state, int x, int y)
{
	GlutWindow *window = getCurrentWindow();

	if(state == GLUT_DOWN)
		window->MousePressed(button, state, x, y);

	if(state == GLUT_UP)
		window->MouseReleased(button);

	window->MouseEvent(window->getMouseButtons(), window->getMouseButtonsFirstTime(), x, y);
}

static void _MotionFunc(int x, int y)
{
	GlutWindow *window = getCurrentWindow();

	window->MouseMove();
	window->MouseEvent(window->getMouseButtons(), window->getMouseButtonsFirstTime(), x, y);
}
static void _PassiveMotionFunc(int x, int y)
{
	getCurrentWindow()->MouseEvent(0, 0, x, y);
}

//static void _EntryFunc(  int ) { }

static void _KeyboardUpFunc(unsigned char key, int x, int y)
{
	getCurrentWindow()->KeyEvent(key, GLUT_UP, glutGetModifiers(), x, y);
}

static void _SpecialUpFunc(int key, int x, int y)
{
	getCurrentWindow()->SpecialKeyEvent(key, GLUT_UP, glutGetModifiers(), x, y);
}

GlutApplication::GlutApplication(int *argc, char **argv)
{
	glutInit(argc, argv);

	glutIdleFunc(_glutIdleFunc);

#ifndef NO_FREEGLUT
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);	
#endif
}

void GlutApplication::setOpenGLVersion(int major, int minor, bool useDebugContext /*= false*/)
{
#ifndef NO_FREEGLUT
	glutInitContextVersion( major, minor);
	if(useDebugContext) {
		glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
	} else {
		glutInitContextFlags(GLUT_FORWARD_COMPATIBLE );
	}
#endif
}

void GlutApplication::addWindow(GlutWindow *window)
{
	if(window == NULL)
		return;

	//ignore if already added
	std::list< int >::const_iterator it = g_windows.begin();
	std::list< int >::const_iterator end = g_windows.end();
	for(; it != end; it++) {
		if(*it == window->getWindowID())
			return;
	}
	glutInitDisplayMode(window->m_mode);
	
	window->m_windowID = glutCreateWindow(window->m_title.c_str());

	glutSetWindow(window->getWindowID());
	setWindowData( window );
	g_windows.push_back(window->getWindowID());

#ifndef NO_FREEGLUT
	glutCloseFunc(_CloseFunc);
#endif
	
	glutKeyboardFunc		(_KeyboardFunc );
	glutSpecialFunc			(_SpecialFunc );
	glutReshapeFunc			(_ReshapeFunc );
	glutDisplayFunc			(_DisplayFunc   );
	glutMouseFunc			(_MouseFunc  );
	glutMotionFunc			(_MotionFunc   );
	glutPassiveMotionFunc	(_PassiveMotionFunc   );
	glutKeyboardUpFunc		(_KeyboardUpFunc  );
	glutSpecialUpFunc		(_SpecialUpFunc   );

#ifdef _DEBUG
	std::cout << "New OpenGL (Vendor:" << glGetString(GL_VENDOR)
			  << " " << glGetString(GL_VERSION) << ") window created" << std::endl;
#endif

	window->LoadEvent();
	window->Update(g_sysTimer.GetTime());
}

void GlutApplication::Run()
{
	glutMainLoop();
}


//////////////////////////////////////////////////////////////////////////
// Window Class
//////////////////////////////////////////////////////////////////////////
GlutWindow::GlutWindow(const std::string & title,  int mode /*= GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH */)
:
	m_title(title),
	m_mode(mode)
{
	m_mousebuttons = 0;
	m_mousefirsttime = 0;
}

void GlutWindow::MousePressed(int button, int state, int x , int y)
{
	if(button < 0 || button > sizeof(m_mousebuttons)*8)
		return;

	if(state == GLUT_DOWN && (m_mousebuttons & (1<<button)) == 0) {
		m_mousefirsttime |= 1<<button;
	} else {
		m_mousefirsttime  &=  ~(1<<(button));
	}
	m_mousebuttons |= 1<<(button);
}

void GlutWindow::MouseReleased(int button)
{
	m_mousebuttons &=  ~(1<<(button));
	m_mousefirsttime  &=  ~(1<<(button));
}

void GlutWindow::MouseMove()
{
	m_mousefirsttime  = 0;
}

void GlutWindow::Close()
{
	int old_window = glutGetWindow();

	glutSetWindow(m_windowID);

	_CloseFunc();

	glutDestroyWindow(m_windowID);

	glutSetWindow(old_window);
}
