#include <string>

#include "AdvancedMaterial.h"

using namespace std;

TextureReflectionMaterial::TextureReflectionMaterial(const std::string &path)
:	Material("TextureReflection")
{
	string vshader(path);
	vshader.append("/shaders/texturereflection.glslv");
	string fshader(path);
	fshader.append("/shaders/texturereflection.glslf");

	GLShader shader;
	shader.loadFromFile(vshader, fshader);

	loadShader(shader);

	glCheckandPrintError();
}

void TextureReflectionMaterial::setPredefinedParameters(const World &world)
{
	Material::setPredefinedParameters(world);
}

void TextureReflectionMaterial::use(const Matrix & model_matrix, const World &world)
{
	Material::use(model_matrix, world);
}

void TextureReflectionMaterial::unuse()
{
	// nothing to do
}

void TextureReflectionMaterial::setTexture(GLTexture2D *texture)
{
	m_shader.getTextureWorkingSet().addTexture("txtColor", texture);
}

void TextureReflectionMaterial::setCubeMap(GLTextureCubeMap *texture)
{
	m_shader.getTextureWorkingSet().addTexture("txtEnvMap", texture);
}

void TextureReflectionMaterial::setDiffuseSpecCoef(float diffuse, float specular, float ambient)
{
	m_shader.setUniform("diffuse", diffuse);
	m_shader.setUniform("specular", specular);
	m_shader.setUniform("ambient", ambient);
}


// TASK 3a)
// Create a new material class named 'TextureNormalMaterial',
// similar to the 'TextureReflectionMaterial' class.
// Load the corresponding glslv and glslf shader files and set the
// material's name to 'TextureNormal' (passed to the constructor of the 'Material' class)
// ~TASK

// SOLUTION
TextureNormalMaterial::TextureNormalMaterial(const std::string &path)
:	Material("TextureNormal")
{
	string vshader(path);
	vshader.append("/shaders/texturenormal.glslv");
	string fshader(path);
	fshader.append("/shaders/texturenormal.glslf");

	GLShader shader;
	shader.loadFromFile(vshader, fshader);

	loadShader(shader);

	glCheckandPrintError();
}

void TextureNormalMaterial::setPredefinedParameters(const World &world)
{
	Material::setPredefinedParameters(world);
}

void TextureNormalMaterial::use(const Matrix & model_matrix, const World &world)
{
	Material::use(model_matrix, world);
}

void TextureNormalMaterial::unuse()
{
	// nothing to do
}

void TextureNormalMaterial::setTexture(GLTexture2D *texture)
{
	m_shader.getTextureWorkingSet().addTexture("txtColor", texture);
}

void TextureNormalMaterial::setNormalMap(GLTexture2D *texture)
{
	m_shader.getTextureWorkingSet().addTexture("txtNormal", texture);
}

void TextureNormalMaterial::setDiffuseSpecCoef(float diffuse, float specular, float ambient)
{
	m_shader.setUniform("diffuse", diffuse);
	m_shader.setUniform("specular", specular);
	m_shader.setUniform("ambient", ambient);
}
// ~SOLUTION
