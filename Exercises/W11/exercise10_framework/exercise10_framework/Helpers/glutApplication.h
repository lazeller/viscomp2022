#ifndef __glutApplication_h__
#define __glutApplication_h__

#include <string>
#include "opengl.h"

class GlutWindow;

class GlutApplication
{
public:
	GlutApplication(int *argc, char **argv);

	//////////////////////////////////////////////////////////////////////////
	// Configuration
	//////////////////////////////////////////////////////////////////////////
	void setOpenGLVersion(int major, int minor, bool useDebugContext = false);

	//////////////////////////////////////////////////////////////////////////
	// Window Functions
	//////////////////////////////////////////////////////////////////////////
	void addWindow(GlutWindow *window);
	
	void Run();
};

class GlutWindow
{
	friend void GlutApplication::addWindow(GlutWindow *window);

public:
	GlutWindow(const std::string &title, int mode = GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	virtual ~GlutWindow() {}

	//////////////////////////////////////////////////////////////////////////
	// mandatory window implementations
	//////////////////////////////////////////////////////////////////////////
	virtual void Draw(float time) = 0;
	virtual void Update(float time) = 0;

	//////////////////////////////////////////////////////////////////////////
	// optional Events
	//////////////////////////////////////////////////////////////////////////
	virtual void LoadEvent() {}
	virtual void ReshapeEvent(int width, int height) { glViewport(0, 0, width, height); }
	virtual void KeyEvent(unsigned char key, int first_time, int modifiers , int x, int y) {}
	virtual void SpecialKeyEvent(unsigned char key, int state, int modifiers , int x, int y) {}
	virtual void MouseEvent(unsigned int buttons, int states, int x, int y) {}

	virtual void CloseEvent() {}


	//////////////////////////////////////////////////////////////////////////
	// properties and commands
	//////////////////////////////////////////////////////////////////////////
	std::string getTitle() const { return m_title; }
	int getWindowID() const { return m_windowID; }

	int getMouseButtons() const { return m_mousebuttons; }
	int getMouseButtonsFirstTime() const { return m_mousefirsttime; }
	
	bool isDestroyed() const { return m_destroyed; }

	void Close();

	//////////////////////////////////////////////////////////////////////////
	// internal function, do not call these functions
	//////////////////////////////////////////////////////////////////////////
	void MousePressed(int button, int state, int x , int y);
	void MouseReleased(int button);
	void MouseMove();
	void SetDestroyed(bool val) { m_destroyed = val; }

private:
	int m_windowID;
	int m_mode;

	std::string m_title;

	bool m_destroyed;

	unsigned int m_mousebuttons;
	unsigned int m_mousefirsttime;
};

#endif // __glutApplication_h__
