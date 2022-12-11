#ifndef Textures_h__
#define Textures_h__

#include <vector>
#include <map>
#include <string>

#include "opengl.h"
#include "Image.h"

class GLShader;

class GLTexture
{
public:
	GLTexture(GLenum target, GLint preferred_unit = -1);

	virtual ~GLTexture() {}

	GLuint getTextureID() const { return m_texid; }
	GLuint getPreferredUnit() const {return m_preferred_unit; }

	// Loads the texture to texture unit 'unit'
	void loadToUnit(GLint unit);

	void createMipmaps();

protected:
	GLuint m_texid;
	GLenum m_target;
	GLint m_preferred_unit;
};

class GLTexture2D: public GLTexture
{
public:
	GLTexture2D(GLint preferred_unit = -1) : GLTexture(GL_TEXTURE_2D, preferred_unit) {}

	void loadFromImage(const Image &img);

private:
	std::string file_name;
};

class GLTextureCubeMap: public GLTexture
{
public:
	GLTextureCubeMap(GLint preferred_unit = -1) : GLTexture(GL_TEXTURE_CUBE_MAP, preferred_unit) {}

	void loadFromImage(GLenum face, const Image &img);
};

class TextureWorkingSet 
{
public:
	TextureWorkingSet();

	bool addTexture(const std::string &name, GLTexture *texture);
	void clear();

	void prepareFor(GLShader &shader);
	void unuse();

private:
	std::vector<GLTexture *> m_units;
	std::map<const std::string, GLint> m_nameToUnit;
};

#endif // Textures_h__
