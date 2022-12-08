#ifndef __Model_h__
#define __Model_h__

#include <map>
#include <string>
#include "DrawableModel.h"
#include "World.h"

#include "opengl.h"

struct __InternalModelGroupInfo;

// A 3d model consisting of multiple groups. Each group can have a different material.
class Model : public DrawableModel
{
public:
	typedef std::map<std::string, __InternalModelGroupInfo *>::const_iterator  ModelGroupIterator;

	Model();

	Model(const std::string &filename);
	virtual ~Model();

	bool loadFromFile(const std::string &filename);

	ModelGroupIterator GroupsBegin();
	ModelGroupIterator GroupsEnd();

	ModelGroupIterator findGroup(const std::string &name);
	const std::string &getGroupName(const ModelGroupIterator iter);

	bool setGroupColor(const ModelGroupIterator &group_iter, const Vector &color);
	const Vector& getGroupColor(const ModelGroupIterator &group_iter);

	bool drawGroup(const std::string &name) { return drawGroup(findGroup(name)); }
	bool drawGroup(const ModelGroupIterator &group_iter);


	//////////////////////////////////////////////////////////////////////////
	// Implementation of DrawableModel
	//////////////////////////////////////////////////////////////////////////
	virtual void draw(World &world, DrawPass pass);

	virtual void setDefaultColor(Vector color) { m_default_color = color; }
	virtual Vector getDefaultColor(Vector color) { return m_default_color; }


protected:
	Vector m_default_color;

private:
	typedef std::map< std::string, __InternalModelGroupInfo * > ModelGroupMap;
	ModelGroupMap m_modelgroups;

	//////////////////////////////////////////////////////////////////////////
	// OpenGL implementation
	//////////////////////////////////////////////////////////////////////////
	GLuint m_vertexbuffer;
	GLuint m_indexbuffer;
};

#endif // __Model_h__
