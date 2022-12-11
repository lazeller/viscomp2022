#include <assert.h>
#include <stdlib.h>

#include "Textures.h"
#include "GLShader.h"

using namespace std;

GLTexture::GLTexture(GLenum target, GLint preferred_unit /*= -1*/)
{
	glCheckandPrintError();

	glGenTextures(1, &m_texid);
	m_target = target;
	m_preferred_unit = preferred_unit;

	if(m_preferred_unit < 0)
	{
		// Select random texture unit:
		GLint nbr_texture_units = 4;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &nbr_texture_units);
		m_preferred_unit = rand() % nbr_texture_units;
	}

	glCheckandPrintError();

	glActiveTexture(m_preferred_unit + GL_TEXTURE0);

	glCheckandPrintError();

	glBindTexture(m_target, m_texid);

	glCheckandPrintError();
}

void GLTexture::loadToUnit(GLint unit)
{
	if(unit >= 0)
		m_preferred_unit = unit;

	glActiveTexture(m_preferred_unit + GL_TEXTURE0);
	glBindTexture(m_target, m_texid);
}

void GLTexture::createMipmaps()
{
	glCheckandPrintError();

	glActiveTexture(m_preferred_unit + GL_TEXTURE0);
	glBindTexture(m_target, m_texid);

	glCheckandPrintError();
}

void GLTexture2D::loadFromImage(const Image &img)
{
	glCheckandPrintError();

	glActiveTexture(m_preferred_unit + GL_TEXTURE0);
	glBindTexture(m_target, m_texid);

	glTexParameteri(m_target, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexParameterf(m_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(m_target, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(m_target, 0, GL_RGB, img.getW(), img.getH(),0, GL_RGB, GL_UNSIGNED_BYTE, img.getData());
	file_name = img.getFilename();

	glCheckandPrintError();
}

void GLTextureCubeMap::loadFromImage(GLenum face, const Image &img)
{
	glCheckandPrintError();

	glActiveTexture(m_preferred_unit + GL_TEXTURE0);
	glBindTexture(m_target, m_texid);

	glTexParameteri(m_target, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexParameterf(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(face, 0, GL_RGB, img.getW(), img.getH(),0, GL_RGB, GL_UNSIGNED_BYTE, img.getData());

	glCheckandPrintError();
}

TextureWorkingSet::TextureWorkingSet()
{
	clear();
}

bool TextureWorkingSet::addTexture(const std::string &name, GLTexture *texture)
{
	if(texture == NULL)
		return false;

	std::map<const std::string, GLint>::const_iterator it;
	it = m_nameToUnit.find(name);
	if( it != m_nameToUnit.end()) {
		m_units[it->second] = NULL;
	}

	//find a free spot
	int idxPreferred = texture->getPreferredUnit() % m_units.size();
	int use = -1;

	int i = idxPreferred;

	do {
		if(m_units[i] == NULL) {
			use = i;
			break;
		}
		i = (i+1) % m_units.size();
	} while( i != idxPreferred);

	if(use == -1)
		return false;

	m_units[use] = texture;
	m_nameToUnit[name] = use;

	return true;
}

void TextureWorkingSet::clear()
{
	GLint nbr_texture_units=4;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &nbr_texture_units);
	m_units.resize(nbr_texture_units);

	m_nameToUnit.clear();
}

void TextureWorkingSet::prepareFor(GLShader &shader)
{
	std::map<const std::string, GLint>::iterator it = m_nameToUnit.begin();
	std::map<const std::string, GLint>::iterator end = m_nameToUnit.end();

	for(; it != end; ++it)
	{
		const std::string &uniformName = it->first;
		GLint unitIdx = it->second;

		assert(m_units[unitIdx] != NULL);
		m_units[unitIdx]->loadToUnit(unitIdx);
		shader.setUniform(uniformName, unitIdx);
	}
}

void TextureWorkingSet::unuse()
{
	//not yet
}
