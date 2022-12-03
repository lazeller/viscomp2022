#ifndef __opengl_h__
#define __opengl_h__

// Include some output header
#include <iostream>

// Include OpenGL
#include <GLee.h>

#if defined(__APPLE__) || defined(__APPLE_CC__)
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>    // Also includes GL/gl.h and GL/glu.h
#endif

#define glCheckandPrintError()	do { \
									GLenum err = glGetError(); \
									if(err != GL_NO_ERROR) \
									{ \
										std::cerr << "OPENGL_ERROR: " << __FILE__ << ", line " << __LINE__ << ": "; \
										std::cerr << gluErrorString(err) << " (code " << err << ")" << std::endl; \
									} \
								} while(0)

#endif // __opengl_h__
