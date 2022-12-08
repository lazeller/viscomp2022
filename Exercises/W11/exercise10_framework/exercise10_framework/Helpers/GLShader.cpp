#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

#include "opengl.h"

#include "GLShader.h"

using namespace std;

// Uses a program as long as the AutoUseProgram object exists
class AutoUseProgram
{
public:
	AutoUseProgram(GLint programID) {
		glGetIntegerv(GL_CURRENT_PROGRAM, &m_oldPogramID);
		glUseProgram(programID);
	}

	virtual ~AutoUseProgram() {
		glUseProgram(m_oldPogramID);
	}

private:
	GLint m_oldPogramID;
};


std::map<GLint, int> GLShader::s_refcount;

//=======================================================================
// Reads a text file
//=======================================================================
// IN:  Name of the file
// OUT: Contents of the file as a string
//=======================================================================
string GLShader::readTextFile(const string &fileName)
{
	// open file
	ifstream ifs(fileName.c_str(), ios::binary);
	if(!ifs)
	{
		cerr << "File not found" << endl;
		return "";
	}

	// Determine length of file
	ifs.seekg(0, ios::end);
	size_t fileLen = ifs.tellg();
	ifs.seekg(0, ios::beg);

	// Allocate string buffer and read string from file
	char *pStr = new char[fileLen + 1];
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
GLuint GLShader::createShaderObject(const string &vertex_file, const string &fragment_file)
{
	// create both shader objects
	m_vshader = glCreateShader(GL_VERTEX_SHADER);
	m_fshader = glCreateShader(GL_FRAGMENT_SHADER);

	m_vertexfile = vertex_file;
	m_fragmentfile = fragment_file;

	// load source code from file an add to shader objects
	string sourceCode = readTextFile(vertex_file);
	if(sourceCode.length() == 0)
	{
		cerr << "create Shader Object failed";
		return 0;
	}
	const char *pS = sourceCode.c_str();
	glShaderSource(m_vshader, 1, &pS, NULL);

	sourceCode = readTextFile(fragment_file);
	if(sourceCode.length() == 0)
	{
		cerr << "create Shader Object failed";
		return 0;
	}
	pS = sourceCode.c_str();
	glShaderSource(m_fshader, 1, &pS, NULL);

	// create Program object
	GLuint program = glCreateProgram();

	// attach both shaders
	glAttachShader(program, m_vshader);
	glAttachShader(program, m_fshader);

	//refcount
	if(program != 0) s_refcount[program] = 1;

	return program;
}

bool GLShader::loadFromFile(string const&vshader_filename, string const&fshader_filename)
{
	m_programID = createShaderObject(vshader_filename, fshader_filename);
	glCheckandPrintError();

	recompile();
	glCheckandPrintError();

	return m_programID != 0;
}

GLShader::GLShader( )
{
	m_programID = 0;
}


void GLShader::use()
{
	//prepare textures in working set
	m_textureSet.prepareFor(*this);

	GLint status = 0;
	glGetProgramiv(m_programID, GL_LINK_STATUS, &status);
	if(status == GL_TRUE)
		glUseProgram(m_programID);
}

void GLShader::unuseAll()
{
	// reactivate fixed function pipeline
	// (note: ffp is deprecated in opengl3.0 and not implmented in opengles 2.0
	glUseProgram(0);
}

GLint GLShader::getUniformLocation(const std::string &name) const
{
	GLint loc = glGetUniformLocation(m_programID, name.c_str());

	return loc;
}

GLint GLShader::getAttributeLocation(const std::string &name) const
{
	GLint loc = glGetAttribLocation(m_programID, name.c_str());

	return loc;
}

void GLShader::setUniform(GLint location, float value)
{
	AutoUseProgram autoUseProgram(m_programID);

	glUniform1f(location, value);
}

void GLShader::setUniform(GLint location, double value)
{
	AutoUseProgram autoUseProgram(m_programID);

	glUniform1f(location, (float)value);
}

void GLShader::setUniform(GLint location, int value)
{
	AutoUseProgram autoUseProgram(m_programID);	

	glUniform1i(location, value);
}

void GLShader::setUniform(GLint location, const Vector &value)
{
	AutoUseProgram autoUseProgram(m_programID);

	glUniform4fv(location, 1, value.getArray());
}

void GLShader::setUniform(GLint location, const Matrix &value)
{
	AutoUseProgram autoUseProgram(m_programID);

	glUniformMatrix4fv(location, 1, true, value.getCArray());
}

void GLShader::getUniform(GLint location, float &value) const
{
	float buffer[16]; //enough space for all cases

	glGetUniformfv(m_programID, location , buffer );

	value = buffer[0];

}

void GLShader::getUniform(GLint location, double &value) const
{
	float buffer[16]; //enough space for all cases

	glGetUniformfv(m_programID, location , buffer );

	value = (double)buffer[0];
}

void GLShader::getUniform(GLint location, int &value) const
{
	int buffer[16]; //enough space for all cases

	glGetUniformiv(m_programID, location , buffer );

	value = buffer[0];
}

void GLShader::getUniform(GLint location, Vector &value) const
{
	float buffer[16]; //enough space for all cases

	glGetUniformfv(m_programID, location , buffer );

	value = Vector(buffer);
}

void GLShader::getUniform(GLint location, Matrix &value) const
{
	float buffer[16]; //enough space for all cases

	glGetUniformfv(m_programID, location, buffer );

	value = Matrix(buffer).transposed();
}

void GLShader::setAttributeDefaultValue(GLint location, float value)
{
	AutoUseProgram autoUseProgram(m_programID);

	glVertexAttrib1f(location, value);
}

void GLShader::setAttributeDefaultValue(GLint location, double value)
{
	AutoUseProgram autoUseProgram(m_programID);

	glVertexAttrib1d(location, value);
}

void GLShader::setAttributeDefaultValue(GLint location, const Vector &value)
{	
	AutoUseProgram autoUseProgram(m_programID);

	glVertexAttrib4fv(location, value.getArray());
}


void GLShader::getAttributeDefaultValue(GLint location, float &value) const
{
	AutoUseProgram autoUseProgram(m_programID);

	float buffer[4]; //enough space for all cases

	glGetVertexAttribfv(location, GL_CURRENT_VERTEX_ATTRIB, buffer );

	value = buffer[0];
}

void GLShader::getAttributeDefaultValue(GLint location, double &value) const
{
	AutoUseProgram autoUseProgram(m_programID);

	double buffer[4]; //enough space for all cases

	glGetVertexAttribdv(location, GL_CURRENT_VERTEX_ATTRIB, buffer );

	value = buffer[0];
}

void GLShader::getAttributeDefaultValue(GLint location, Vector &value) const
{
	AutoUseProgram autoUseProgram(m_programID);

	float buffer[4]; //enough space for all cases

	glGetVertexAttribfv(location, GL_CURRENT_VERTEX_ATTRIB, buffer );

	value = Vector(buffer);
}

void GLShader::bindAttributesLocations(const std::map <const std::string, GLint> &nameToLoc)
{
	map<const std::string, GLint>::const_iterator iter = nameToLoc.begin();
	map<const std::string, GLint>::const_iterator end = nameToLoc.end();

	for(; iter != end; iter++)
	{
		glBindAttribLocation(m_programID, iter->second, iter->first.c_str());
	}
}

void GLShader::recompile()
{
	GLint status;

	// compile both shaders
	glCompileShader(m_vshader);
	glGetShaderiv(m_vshader, GL_COMPILE_STATUS, &status);
	if(status == 0)
	{
		GLint length;
		glGetShaderiv(m_vshader, GL_INFO_LOG_LENGTH, &length);
		char * errorstr = new char[length];
		glGetShaderInfoLog(m_vshader, length, NULL, errorstr);
		cerr << m_vertexfile << " compile error(s): " << endl << errorstr << endl;
		delete [] errorstr;
		return;
	}

	glCompileShader(m_fshader);
	glGetShaderiv(m_fshader, GL_COMPILE_STATUS, &status);
	if(status == 0)
	{
		GLint length;
		glGetShaderiv(m_fshader, GL_INFO_LOG_LENGTH, &length);
		char * errorstr = new char[length];
		glGetShaderInfoLog(m_fshader, length, NULL, errorstr);
		cerr << m_fragmentfile << " compile error(s): " << endl << errorstr << endl;
		delete[] errorstr;
		return;
	}

	// Link Program
	glLinkProgram(m_programID);
	glGetProgramiv(m_programID, GL_LINK_STATUS, &status);
	if(status == 0)
	{
		GLint length;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &length);
		char * errorstr = new char[length];
		glGetProgramInfoLog(m_programID, length, NULL, errorstr);
		cerr << m_vertexfile << " " << m_fragmentfile << " link error(s): " << endl << errorstr << endl;
		delete[] errorstr;
		return;
	}
}

GLShader::~GLShader()
{
	if(m_programID != 0) {
		s_refcount[m_programID]--;

		if(s_refcount[m_programID] == 0)
			deleteShader();
	}
}

void GLShader::deleteShader()
{
	assert(s_refcount[m_programID] == 0);

	if(m_programID)
	{
		glDeleteProgram(m_programID);
		m_programID = 0;
	}

	if(m_vshader)
	{
		glDeleteShader(m_vshader);
		m_vshader = 0;
	}

	if(m_fshader)
	{
		glDeleteShader(m_fshader);
		m_fshader = 0;
	}
}

const GLShader& GLShader::operator=(const GLShader &other)
{
	//delete ours first
	if(m_programID != 0) {
		s_refcount[m_programID]--;

		if(s_refcount[m_programID] == 0)
			deleteShader();
	}

	m_programID = other.m_programID;
	m_vshader = other.m_vshader;
	m_fshader = other.m_fshader;
	m_vertexfile = other.m_vertexfile;
	m_fragmentfile = other.m_fragmentfile;

	if(m_programID != 0)
		s_refcount[m_programID]++;

	return *this;
}

GLShader::GLShader(const GLShader &other)
{
	m_programID = other.m_programID;
	m_vshader = other.m_vshader;
	m_fshader = other.m_fshader;
	m_vertexfile = other.m_vertexfile;
	m_fragmentfile = other.m_fragmentfile;

	if(m_programID != 0)
		s_refcount[m_programID]++;
}
