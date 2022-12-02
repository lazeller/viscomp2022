#ifndef __GLShader_h__
#define __GLShader_h__
#define GL_GLEXT_PROTOTYPES
#include <string>
#include <map>
#include "opengl.h"

// Contains a shader program consisting of a vertex shader and a fragment shader
class GLShader
{
public:
	GLShader();

	virtual ~GLShader();

	// Loads the shader program from a vertex shader file and a fragment shader file
	bool loadFromFile(const std::string &vshader_filename, const std::string &fshader_filename);

	// Uses the shader program
	bool use();

	// Un-uses any current shader program
	static void unuseAll();
	
	// Returns the index of a uniform with a specific name. Returns -1 if not found.
	GLint getUniformLocation(const std::string &name) const;

	// Returns the index of an attribute with a specific name. Returns -1 if not found.
	GLint getAttributeLocation(const std::string &name) const;

	//////////////////////////////////////////////////////////////////////////
	// Uniform Set and Get
	//////////////////////////////////////////////////////////////////////////
	// Sets the float uniform at index 'location' to v
	void setUniform(GLint location, float v);

	// Sets the vec2 uniform at index 'location' to (v1, v2)
	void setUniform(GLint location, float v1, float v2);

	// Sets the vec3 uniform at index 'location' to (v1, v2, v3)
	void setUniform(GLint location, float v1, float v2, float v3);

private:
	// Compiles the shaders
	bool recompile();

	// Returns the contents of the text file 'filename'. Returns an empty string if failed.
	static std::string readTextFile(const std::string &filename);

	/* Creates the shader objects and program (m_vshader, m_fshader, m_programID) from
		the given vertex- and fragment-shader files. */
	bool createShaderObject(const std::string &vertex_file, const std::string &fragment_file);

	// Deletes both shader objects and the shader program
	void deleteShader();

	// Disallow copy constructor:
	GLShader(const GLShader &other) {
	}

	// Disallow assignment operator:
	const GLShader &operator=(const GLShader &other) {
		return *this;
	}
	
private:
	GLuint m_vshader;
	GLuint m_fshader;
	GLuint m_programID;

	std::string m_vertexfile;
	std::string m_fragmentfile;
};

#endif // __GLShader_h__
