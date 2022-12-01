#include "Matrix.h"
#include "Material.h"
#include "World.h"

using namespace std;


//////////////////////////////////////////////////////////////////////////
// Helper Macro
//////////////////////////////////////////////////////////////////////////
#define TRY_TO_SET_UNIFORM(name, value)				\
	do {											\
		GLint loc;									\
		loc = m_shader.getUniformLocation(name);	\
		if(loc != -1) m_shader.setUniform(loc, value);	\
	} while(0)


//////////////////////////////////////////////////////////////////////////
// Base Material
//////////////////////////////////////////////////////////////////////////

void Material::setPredefinedParameters( const World &world )
{
	// Okay we do it the straight forward way
	// which is not fast but works

	//View and Projection Matrix:
	TRY_TO_SET_UNIFORM("w_ViewMatrix", world.getViewMatrix());
	TRY_TO_SET_UNIFORM("w_ProjectionMatrix", world.getProjectionMatrix());
	TRY_TO_SET_UNIFORM("w_ProjectionViewMatrix", world.getProjectionViewMatrix());

	//Camera Position
	TRY_TO_SET_UNIFORM("w_CameraPosition", world.getCameraPosition());

	// one light
	TRY_TO_SET_UNIFORM("w_LightPosition", world.getLightPosition(0));

	//lights
	assert(WORLD_NUMLIGHTS <= 9);  //change implementation in the for loop
	// to support so many lights
	char name[] = "w_LightPositions[x]";
	int xpos = 17;

	for(int i = 0; i < WORLD_NUMLIGHTS; i++) {
		name[xpos] = '0' + i;
		TRY_TO_SET_UNIFORM(name, world.getLightPosition(i));
	}
}

void Material::use( const Matrix & model_matrix, const World &world )
{
	//start using this shader
	m_shader.use();

	//prepare shader for use
	//for simple default material this means setting some model matrices.
	TRY_TO_SET_UNIFORM("m_ModelMatrix", model_matrix);
	TRY_TO_SET_UNIFORM("m_ModelNormalMatrix", model_matrix.normaltransform());

	TRY_TO_SET_UNIFORM("m_ViewModelMatrix", world.getViewMatrix() * model_matrix);
	TRY_TO_SET_UNIFORM("m_ProjectionViewModelMatrix", world.getProjectionViewMatrix() * model_matrix);
}

void Material::unuse()
{
	//do nothing
}

Material::Material( GLShader & material_Shader )
{
	loadShader(material_Shader);
}

void Material::loadShader(GLShader & material_Shader)
{
	m_shader = material_Shader;

	//Default Locations
	static map <const string, GLint> binding;
	if(binding.size() == 0) {
		binding["position"] = DEFAULT_VERTEX_POS_BINDING;
		binding["texcoord"] = DEFAULT_VERTEX_TEX_BINDING;
		binding["normal"]  = DEFAULT_VERTEX_NORMAL_BINDING;
	}

	m_shader.bindAttributesLocations(binding);
	glCheckandPrintError();
	m_shader.recompile();
	glCheckandPrintError();
}
//////////////////////////////////////////////////////////////////////////
// Base Material
//////////////////////////////////////////////////////////////////////////
ColoredDiffuseMaterial::ColoredDiffuseMaterial( const std::string &path /*= ""*/ )
	: Material("ColoredDiffuse")
{
	string vshader(path);
	vshader.append("/diffusecolor.glslv");
	string fshader(path);
	fshader.append("/diffusecolor.glslf");

	GLShader shader;
	shader.loadFromFile(vshader, fshader);

	loadShader(shader);
	glCheckandPrintError();
}

void ColoredDiffuseMaterial::setPredefinedParameters( const World &world )
{
	Material::setPredefinedParameters(world);
}

void ColoredDiffuseMaterial::use( const Matrix & model_matrix, const World &world )
{
	TRY_TO_SET_UNIFORM("color", m_color);

	Material::use(model_matrix, world);
}

void ColoredDiffuseMaterial::unuse()
{

}

void ColoredDiffuseMaterial::setColor(Vector val)
{
	m_color = val;

	TRY_TO_SET_UNIFORM("color", m_color);
}
