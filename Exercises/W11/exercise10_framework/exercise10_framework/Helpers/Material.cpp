#include "Matrix.h"
#include "Material.h"
#include "World.h"

using namespace std;


//////////////////////////////////////////////////////////////////////////
// Base Material
//////////////////////////////////////////////////////////////////////////

void Material::setPredefinedParameters(const World &world)
{
	// Okay we do it the straight forward way
	// which is not fast but works	

	//View and Projection Matrix:
	m_shader.setUniform("w_ViewMatrix", world.getViewMatrix());
	m_shader.setUniform("w_ProjectionMatrix", world.getProjectionMatrix());
	m_shader.setUniform("w_ProjectionViewMatrix", world.getProjectionViewMatrix());

	//Camera Position
	m_shader.setUniform("w_CameraPosition", world.getCameraPosition());

	// one light
	m_shader.setUniform("w_LightPosition", world.getLightPosition(0));

	//lights
	assert(WORLD_NUMLIGHTS <= 9);  //change implementation in the for loop
	// to support so many lights
	char name[] = "w_LightPositions[x]";
	int xpos = 17;

	for(int i = 0; i < WORLD_NUMLIGHTS; i++) {
		name[xpos] = '0' + i;
		m_shader.setUniform(name, world.getLightPosition(i));
	}
}

void Material::use(const Matrix &model_matrix, const World &world)
{
	//start using this shader
	m_shader.use();

	//prepare shader for use
	//for simple default material this means setting some model matrices.
	m_shader.setUniform("m_ModelMatrix", model_matrix);
	m_shader.setUniform("m_ModelNormalMatrix", model_matrix.normaltransform());

	m_shader.setUniform("m_ViewModelMatrix", world.getViewMatrix() * model_matrix);
	m_shader.setUniform("m_ProjectionViewModelMatrix", world.getProjectionViewMatrix() * model_matrix);

	glCheckandPrintError();
}

void Material::unuse()
{
	//do nothing
}

Material::Material(GLShader &material_Shader)
{
	loadShader(material_Shader);
}

void Material::loadShader(GLShader &material_Shader)
{
	m_shader = material_Shader;

	//Default Locations
	static map <const string, GLint> binding;
	if(binding.size() == 0) {
		binding["position"] = DEFAULT_VERTEX_POS_BINDING;
		binding["texcoord"] = DEFAULT_VERTEX_TEX_BINDING;
		binding["normal"] = DEFAULT_VERTEX_NORMAL_BINDING;
		binding["tangent"] = DEFAULT_VERTEX_TANGENT_BINDING;
		binding["bitangent"] = DEFAULT_VERTEX_BITANGENT_BINDING;
	}

	m_shader.bindAttributesLocations(binding);
	glCheckandPrintError();
	m_shader.recompile();
	glCheckandPrintError();
}
//////////////////////////////////////////////////////////////////////////
// Base Material
//////////////////////////////////////////////////////////////////////////
ColoredPhongMaterial::ColoredPhongMaterial(const std::string &path)
	: Material("ColoredPhong")
{
	string vshader(path);
	vshader.append("/shaders/phong.glslv");
	string fshader(path);
	fshader.append("/shaders/phong.glslf");

	GLShader shader;
	shader.loadFromFile(vshader, fshader);

	loadShader(shader);
	glCheckandPrintError();
}

void ColoredPhongMaterial::setPredefinedParameters(const World &world)
{
	Material::setPredefinedParameters(world);
}

void ColoredPhongMaterial::use(const Matrix &model_matrix, const World &world)
{
	Material::use(model_matrix, world);
}

void ColoredPhongMaterial::unuse()
{

}

void ColoredPhongMaterial::setColor(const Vector &val)
{
	m_shader.setUniform("color", val);
}

void ColoredPhongMaterial::setDiffuseSpecCoef(float diffuse, float specular, float ambient)
{
	m_shader.setUniform("diffuse", diffuse);
	m_shader.setUniform("specular", specular);
	m_shader.setUniform("ambient", ambient);
}
