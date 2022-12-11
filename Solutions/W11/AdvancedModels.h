#ifndef __AdvancedModels_h__
#define __AdvancedModels_h__

#include <vector>
#include "Helpers/Model.h"
#include "Helpers/Textures.h"


class ETHTower : public Model
{
public:
	ETHTower();

	virtual void draw(World &world, DrawPass pass);

private:
	std::map<const std::string, GLTexture2D*> m_textures;
};


class Helicopter : public Model 
{
public:
	Helicopter();

	virtual void draw(World &world, DrawPass pass);

private:
	std::map<const std::string, GLTexture2D*> m_textures;
};

#endif // __AdvancedModels_h__
