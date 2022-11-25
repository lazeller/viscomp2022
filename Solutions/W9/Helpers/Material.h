#ifndef __Material_h__
#define __Material_h__

#include <string>

#include "Matrix.h"
#include "GLShader.h"

class World;

class Material 
{
public:
	Material(const std::string &materialName) {
		m_materialName = materialName;
	}
	Material(GLShader & material_Shader);

	void loadShader();

	virtual void setPredefinedParameters(const World &world);

	virtual void use(const Matrix & model_matrix, const World &world);

	virtual void unuse();

	GLShader &getShader() { return m_shader; }

	const std::string &getName() const { return m_materialName; }

protected:
	std::string m_materialName;

	GLShader m_shader;

	void loadShader( GLShader & material_Shader);
};


//////////////////////////////////////////////////////////////////////////
// Advanced Materials
//////////////////////////////////////////////////////////////////////////
class ColoredDiffuseMaterial: public Material
{
public:
	ColoredDiffuseMaterial(const std::string &path = "./shaders");

	virtual void setPredefinedParameters(const World &world);

	virtual void use(const Matrix &model_matrix, const World &world);

	virtual void unuse();

	const Vector &getColor() const { return m_color; }
	void setColor(Vector val);

protected:
	Vector m_color;
};

#endif // __Material_h__
