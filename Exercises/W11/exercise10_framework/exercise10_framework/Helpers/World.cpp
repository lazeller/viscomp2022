#include <map>
#include <vector>

#include "World.h"
#include "Matrix.h"


using namespace std;
//////////////////////////////////////////////////////////////////////////
//World
//////////////////////////////////////////////////////////////////////////

World::World(Material *default_material)
{
	addMaterial(default_material);
}

void World::addMaterial(Material *new_material)
{
	m_materials[new_material->getName()] = new_material;
}

Material * World::getMaterial(const std::string &materialName)
{
	MaterialMap::iterator iter = m_materials.find(materialName);
	if(iter == m_materials.end())
		return NULL;

	return iter->second;
}

void World::updatePredefinedMaterialParamters()
{
	MaterialMap::iterator iter = m_materials.begin();
	MaterialMap::iterator end = m_materials.end();

	for(; iter!=end; ++iter) {
		iter->second->setPredefinedParameters(*this);
	}
}

World::~World()
{
	MaterialMap::iterator iter = m_materials.begin();
	MaterialMap::iterator end = m_materials.end();

	for(; iter!=end; ++iter) {
		delete iter->second;
	}

	m_materials.clear();
}
