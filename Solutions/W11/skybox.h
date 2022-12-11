#ifndef skybox_h__
#define skybox_h__

#include "Helpers/DrawableModel.h"
#include "Helpers/Material.h"
#include "Helpers/Textures.h"


class SkyboxMaterial : public Material
{
public:
	SkyboxMaterial(const std::string &path = MAIN_DIR);

	virtual void setPredefinedParameters(const World &world);

	virtual void use(const Matrix &model_matrix, const World &world);

	virtual void unuse();

	//set cubemap texture
	void setSkyboxTexture(GLTextureCubeMap *texture);

private:
};

class Skybox : public DrawableModel 
{
public:
	Skybox();

	void buildGeometry();

	virtual void draw(World &world, DrawPass pass);

private:
	GLuint m_vertexbuffer;
	GLuint m_indexbuffer;
};

#endif // skybox_h__
