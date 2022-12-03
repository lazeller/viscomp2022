#include <fstream>
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


GLShader::GLShader( )
{
	m_vshader = 0;
	m_fshader = 0;
	m_programID = 0;
}

GLShader::~GLShader()
{
	deleteShader();
}

bool GLShader::loadFromFile(string const &vshader_filename, string const &fshader_filename)
{
	if(!createShaderObject(vshader_filename, fshader_filename))
		return false;

	m_vertexfile = vshader_filename;
	m_fragmentfile = fshader_filename;

	if(!recompile())
		return false;

	return true;
}

bool GLShader::use()
{
	GLint status = 0;
	glGetProgramiv(m_programID, GL_LINK_STATUS, &status);

	if(status != GL_TRUE)
	{
		cerr << "Program can only be used after successfully linking";
		return false;
	}

	glUseProgram(m_programID);

	return true;
}

void GLShader::unuseAll()
{
	// reactivate fixed function pipeline
	// (note: ffp is deprecated in opengl3.0 and not implmented in opengles 2.0
	glUseProgram(0);
}

GLint GLShader::getUniformLocation(const std::string &name) const
{
	return glGetUniformLocation(m_programID, name.c_str());
}

GLint GLShader::getAttributeLocation(const std::string &name) const
{
	return glGetAttribLocation(m_programID, name.c_str());
}

void GLShader::setUniform(GLint location, float v)
{
	AutoUseProgram autoUseProgram(m_programID);

	glUniform1f(location, v);
}

void GLShader::setUniform(GLint location, float v1, float v2)
{
	AutoUseProgram autoUseProgram(m_programID);

	glUniform2f(location, v1, v2);
}

void GLShader::setUniform(GLint location, float v1, float v2, float v3)
{
	AutoUseProgram autoUseProgram(m_programID);

	glUniform3f(location, v1, v2, v3);
}

bool GLShader::recompile()
{
	GLint status = 0;

	// compile both shaders
	glCompileShader(m_vshader);
	glGetShaderiv(m_vshader, GL_COMPILE_STATUS, &status);
	if(status == 0)
	{
		GLint length = 0;
		glGetShaderiv(m_vshader, GL_INFO_LOG_LENGTH, &length);
		char *errorstr = new char[length];
		glGetShaderInfoLog(m_vshader, length, NULL, errorstr);
		cerr << m_vertexfile << " compile error(s):" << endl << errorstr;
		delete[] errorstr;
		return false;
	}

	glCompileShader(m_fshader);
	glGetShaderiv(m_fshader, GL_COMPILE_STATUS, &status);
	if(status == 0)
	{
		GLint length = 0;
		glGetShaderiv(m_fshader, GL_INFO_LOG_LENGTH, &length);
		char *errorstr = new char[length];
		glGetShaderInfoLog(m_fshader, length, NULL, errorstr);
		cerr << m_fragmentfile << " compile error(s):" << endl << errorstr;
		delete[] errorstr;
		return false;
	}

	// Link Program
	glLinkProgram(m_programID);
	glGetProgramiv(m_programID, GL_LINK_STATUS, &status);
	if(status == 0)
	{
		GLint length = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &length);
		char *errorstr = new char[length];
		glGetProgramInfoLog(m_programID, length, NULL, errorstr);
		cerr << m_vertexfile << " " << m_fragmentfile << " link error(s):" << endl << errorstr;
		delete[] errorstr;
		return false;
	}

	return true;
}

string GLShader::readTextFile(const string &filename)
{
	// open file
	ifstream ifs(filename.c_str(), ios::binary);
	if(!ifs)
	{
		cerr << "File not found: " << filename << endl;
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

bool GLShader::createShaderObject(const string &vertex_file, const string &fragment_file)
{
	// create both shader objects
	m_vshader = glCreateShader(GL_VERTEX_SHADER);
	m_fshader = glCreateShader(GL_FRAGMENT_SHADER);

	// load source code from file an add to shader objects
	string sourceCode = readTextFile(vertex_file);
	if(sourceCode.length() == 0)
	{
		cerr << "Create Shader Object failed" << endl;
		return false;
	}
	const char *pS = sourceCode.c_str();
	glShaderSource(m_vshader, 1, &pS, NULL);

	sourceCode = readTextFile(fragment_file);
	if(sourceCode.length() == 0)
	{
		cerr << "Create Shader Object failed" << endl;
		return false;
	}
	pS = sourceCode.c_str();
	glShaderSource(m_fshader, 1, &pS, NULL);

	// create Program object
	m_programID = glCreateProgram();

	// attach both shaders
	glAttachShader(m_programID, m_vshader);
	glAttachShader(m_programID, m_fshader);

	return true;
}

void GLShader::deleteShader()
{
	if(m_programID != 0)
	{
		glDeleteProgram(m_programID);
		m_programID = 0;
	}

	if(m_vshader != 0)
	{
		glDeleteShader(m_vshader);
		m_vshader = 0;
	}

	if(m_fshader != 0)
	{
		glDeleteShader(m_fshader);
		m_fshader = 0;
	}
}
