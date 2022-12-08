#ifndef __World_h__
#define __World_h__

#include <string>
#include <vector>
#include <map>

#include "GLShader.h"
#include "Material.h"


//////////////////////////////////////////////////////////////////////////
// Global Configuration
//////////////////////////////////////////////////////////////////////////

const int WORLD_NUMLIGHTS = 4;

//////////////////////////////////////////////////////////////////////////
// Globals
//////////////////////////////////////////////////////////////////////////
const int DEFAULT_VERTEX_POS_BINDING =		 0;
const int DEFAULT_VERTEX_TEX_BINDING =		 1;
const int DEFAULT_VERTEX_NORMAL_BINDING =	 2;
const int DEFAULT_VERTEX_TANGENT_BINDING =	 3;
const int DEFAULT_VERTEX_BITANGENT_BINDING = 4;

const int DEFAULT_MATERIAL = 0;


class World
{
public:
	World() {}

	World(Material *default_material);

	virtual ~World();

	void addMaterial(Material *new_material);
	
	Material * getMaterial(const std::string &materialName);

	int getMaterialCount() const { return (int)m_materials.size(); }

	void updatePredefinedMaterialParamters();
	
	const Matrix& getViewMatrix() const { return m_viewMatrix; }
	Matrix& getViewMatrix() { return m_viewMatrix; }
	void setViewMatrix(Matrix val) { m_viewMatrix = val; }

	const Matrix& getProjectionMatrix() const { return m_projectionMatrix; }
	Matrix& getProjectionMatrix() { return m_projectionMatrix; }
	void setProjectionMatrix(Matrix val) { m_projectionMatrix = val; }

	const Matrix getProjectionViewMatrix() const { return m_projectionMatrix*m_viewMatrix; }

	const Vector& getCameraPosition() const { return m_cameraPosition; }
	Vector& getCameraPosition() { return m_cameraPosition; }

	const Vector& getLightPosition( unsigned int light ) const {	assert(light < WORLD_NUMLIGHTS);
														return m_lightPositions[light]; 
													}
	Vector& getLightPosition( unsigned int light ) { assert(light < WORLD_NUMLIGHTS);
													 return m_lightPositions[light]; 
													}

protected:
	Vector m_cameraPosition;
	Vector m_cameraDirection;
	Vector m_cameraUpVector;
	Vector m_lightPositions[WORLD_NUMLIGHTS];
	Matrix m_viewMatrix;
	Matrix m_projectionMatrix;
	
private:
	typedef std::map<std::string, Material*> MaterialMap;
	MaterialMap m_materials;
};

#endif // __World_h__
