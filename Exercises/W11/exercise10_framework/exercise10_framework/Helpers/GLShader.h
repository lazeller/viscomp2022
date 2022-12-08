#ifndef __GLShader_h__
#define __GLShader_h__

#include <string>
#include <map>
#include "opengl.h"
#include "Vector.h"
#include "Matrix.h"
#include "Textures.h"


class GLShader
{
public:
	GLShader();

	~GLShader();

	//////////////////////////////////////////////////////////////////////////
	// load shader sources
	//////////////////////////////////////////////////////////////////////////
	bool loadFromFile(const std::string &vshader_filename, const std::string &fshader_filename);

	//////////////////////////////////////////////////////////////////////////
	// Ref counting
	//////////////////////////////////////////////////////////////////////////
	GLShader(const GLShader &other);

	const GLShader& operator=(const GLShader &other);

	//////////////////////////////////////////////////////////////////////////
	// Functions
	//////////////////////////////////////////////////////////////////////////
	void use();
	static void unuseAll();
	
	GLint getUniformLocation(const std::string &name) const;
	GLint getAttributeLocation(const std::string &name) const;


	//////////////////////////////////////////////////////////////////////////
	// Uniform Set and Get
	//////////////////////////////////////////////////////////////////////////
	void setUniform(GLint location, float value);
	void setUniform(GLint location, double value);
	void setUniform(GLint location, int value);
	void setUniform(GLint location, const Vector &value);
	void setUniform(GLint location, const Matrix &value);
	template <class T>
	bool setUniform(const std::string &name, const T &value)
	{
		GLint location = getUniformLocation(name);
		if(location == -1)
			return false;

		setUniform(location, value);
		return true;
	}
	
	void getUniform(GLint location, float &value) const;
	void getUniform(GLint location, double &value) const;
	void getUniform(GLint location, int &value) const;
	void getUniform(GLint location, Vector &value) const;
	void getUniform(GLint location, Matrix &value) const;
	template <class T>
	void getUniform(const std::string &name, T &value) const
	{
		GLint location = getUniformLocation(name);
		if(location == -1)
			return false;

		getUniform(location, value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// Attribute Set and Get
	//////////////////////////////////////////////////////////////////////////
	void setAttributeDefaultValue(GLint location, float value);
	void setAttributeDefaultValue(GLint location, double value);
	void setAttributeDefaultValue(GLint location, const Vector &value);
	template <class T>
	bool setAttributeDefaultValue(const std::string &name, const T &value)
	{
		GLint location = getAttributeLocation(name);
		if(location == -1)
			return false;

		setAttributeDefaultValue(location, value);
		return true;
	}

	void getAttributeDefaultValue(GLint location, float &value) const;
	void getAttributeDefaultValue(GLint location, double &value) const;
	void getAttributeDefaultValue(GLint location, Vector &value) const;
	template <class T>
	bool voidgetAttributeDefaultValue(const std::string &name, T &value) const
	{
		GLint location = getAttributeLocation(name);
		if(location == -1)
			return false;

		getAttributeDefaultValue(location, value);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// Bind Attributes to predefined given locations
	// needs recompile
	//////////////////////////////////////////////////////////////////////////
	void bindAttributesLocations(const std::map<const std::string, GLint> &nameToLoc);

	//////////////////////////////////////////////////////////////////////////
	// recompile
	//////////////////////////////////////////////////////////////////////////
	void recompile();

	//////////////////////////////////////////////////////////////////////////
	// get the TextureWorkingSet
	//////////////////////////////////////////////////////////////////////////
	TextureWorkingSet &getTextureWorkingSet() { return m_textureSet; }


private:
	std::string readTextFile(const std::string &fileName);
	GLuint createShaderObject(const std::string &vertex_file, const std::string &fragment_file);

	void deleteShader();
	
	GLuint m_programID;
	GLuint m_vshader, m_fshader;
	TextureWorkingSet m_textureSet;

	std::string m_vertexfile, m_fragmentfile;

	static std::map<GLint, int> s_refcount;
};

#endif // __GLShader_h__
