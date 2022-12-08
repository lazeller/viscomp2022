#ifndef __AdvancedMaterial_h__
#define __AdvancedMaterial_h__

#include "Helpers/Material.h"
#include "Helpers/Textures.h"


class TextureReflectionMaterial : public Material
{
public:
	TextureReflectionMaterial(const std::string &path = MAIN_DIR);

	virtual void setPredefinedParameters(const World &world);

	virtual void use(const Matrix &model_matrix, const World &world);

	virtual void unuse();

	void setTexture(GLTexture2D *texture);
	void setCubeMap(GLTextureCubeMap *texture);
	void setDiffuseSpecCoef(float diffuse, float specular, float ambient);
};

// TASK 3a)
// Create a new material class named 'TextureNormalMaterial',
// similar to the 'TextureReflectionMaterial' class.
// The material should have a method 'setNormalMap()' to set the 2D texture
// containing the normal map.
// ~TASK

#endif // __AdvancedMaterial_h__
