#include <string>

#include "skybox.h"

#include "Helpers/DrawableModel.h"
#include "Helpers/Material.h"
#include "Helpers/Matrix.h"
#include "Helpers/World.h"

using namespace std;

SkyboxMaterial::SkyboxMaterial(const std::string &path)
:	Material("Skybox")
{
	string vshader(path);
	vshader.append("/shaders/skybox.glslv");
	string fshader(path);
	fshader.append("/shaders/skybox.glslf");

	GLShader shader;
	shader.loadFromFile(vshader, fshader);

	loadShader(shader);

	glCheckandPrintError();
}

void SkyboxMaterial::setPredefinedParameters(const World &world)
{
	// nothing to do here
	// Material::setPredefinedParameters(world);
}

void SkyboxMaterial::use(const Matrix &model_matrix, const World &world)
{
	Matrix ipv = world.getProjectionViewMatrix().getNoTranslationMatrix().inversed();
	
	m_shader.setUniform("backprojectMatrix", ipv);

	glDepthFunc(GL_LEQUAL);
	Material::use(model_matrix, world);

	glCheckandPrintError();
}

void SkyboxMaterial::unuse()
{
	glDepthFunc(GL_LESS);
}

void SkyboxMaterial::setSkyboxTexture(GLTextureCubeMap *texture)
{
	m_shader.getTextureWorkingSet().addTexture("txtSky", texture);
}


Skybox::Skybox()
{

}

void Skybox::buildGeometry()
{
	//create buffers
	glGenBuffers(1, &m_vertexbuffer);
	glGenBuffers(1, &m_indexbuffer);

	float vertices[] = 
	{
		 0.f,  2.f, 1.f, 1.f,
		-3.f,  -1.f, 1.f, 1.f,
		3.f,  -1.f, 1.f, 1.f
	};

	GLshort indices[] = {0, 1, 2};


	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(vertices),
		vertices,
		GL_STATIC_DRAW
		);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(indices),
		indices,
		GL_STATIC_DRAW
		);
}

void Skybox::draw(World &world, DrawPass pass)
{
	SkyboxMaterial *material = dynamic_cast<SkyboxMaterial *>(world.getMaterial("Skybox"));
	assert(material);

	if(material == NULL)
		return;

	material->use(m_modelMatrix, world);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexbuffer);
	glEnableVertexAttribArray(DEFAULT_VERTEX_POS_BINDING);
	glVertexAttribPointer(DEFAULT_VERTEX_POS_BINDING, 
		4, GL_FLOAT, false, 
		sizeof(float)*4, NULL);

	glDrawElements(GL_TRIANGLES, 
		3, 
		GL_UNSIGNED_SHORT,
		NULL);

	material->unuse();
}
