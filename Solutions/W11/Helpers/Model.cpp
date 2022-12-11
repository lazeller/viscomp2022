#include <map>
#include <vector>

#include "opengl.h"
#include "Model.h"
#include "World.h"
#include "ObjectPTN.h"


#define BIND_MODELBUFFERS()											\
	GLuint __oldvbuffer, __oldibuffer;								\
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*)&__oldvbuffer);	\
	if(__oldvbuffer != m_vertexbuffer) {							\
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);				\
	}																\
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, (GLint*)&__oldibuffer);	\
	if(__oldibuffer != m_indexbuffer) {								\
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexbuffer);		\
	}

#define BIND_MODELBUFFERS_HASCHANGED() (__oldvbuffer != m_vertexbuffer || __oldibuffer != m_indexbuffer)

#define UNBIND_MODELBUFFERS()										\
	if(__oldvbuffer != 0 && __oldvbuffer != m_vertexbuffer) {		\
		glBindBuffer(GL_ARRAY_BUFFER, __oldvbuffer);				\
	}																\
	if(__oldibuffer != 0 && __oldibuffer != m_indexbuffer) {		\
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, __oldibuffer);		\
	}

struct __InternalModelGroupInfo {
	GLint offset;
	GLint count;

	bool hasColor;
	Vector color;
};


Model::Model() 
{
	m_vertexbuffer = 0;
	m_indexbuffer = 0;
	m_default_color = Vector(0.6f, 0.6f, 0.6f, 1.0f);
}

Model::Model(const std::string &filename)
{
	m_vertexbuffer = 0;
	m_indexbuffer = 0;
	loadFromFile(filename);
}

Model::~Model()
{
	ModelGroupIterator iter = GroupsBegin();
	ModelGroupIterator end = GroupsEnd();

	for(; iter != end; iter++) {
		delete iter->second;
	}

	m_modelgroups.clear();

	glDeleteBuffers(1, &m_vertexbuffer);
	glDeleteBuffers(1, &m_indexbuffer);

}

bool Model::loadFromFile(std::string const &filename)
{
	//
	ObjectPTN loader;

	if(!loader.LoadFromFile(filename))
		return false;

	//create buffers
	glGenBuffers(1, &m_vertexbuffer);
	glGenBuffers(1, &m_indexbuffer);

	BIND_MODELBUFFERS();

	//create vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
	glBufferData(	GL_ARRAY_BUFFER,
		loader.GetNumVertices() * sizeof(PTNVertex),
		loader.GetVertexBufferPtr(),
		GL_STATIC_DRAW
		);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexbuffer);
	glBufferData(	GL_ELEMENT_ARRAY_BUFFER,
		loader.GetNumIndices() * sizeof(int),
		loader.GetIndexBufferPtr(),
		GL_STATIC_DRAW
		);

	glVertexAttribPointer(DEFAULT_VERTEX_POS_BINDING, 
		3, GL_FLOAT, false, 
		sizeof(PTNVertex),(GLvoid*) offsetof(PTNVertex, pos)  );
	glVertexAttribPointer(DEFAULT_VERTEX_TEX_BINDING, 
		2, GL_FLOAT, false, 
		sizeof(PTNVertex), (GLvoid*) offsetof(PTNVertex, texCoords)  );
	glVertexAttribPointer(DEFAULT_VERTEX_NORMAL_BINDING, 
		3, GL_FLOAT, false, 
		sizeof(PTNVertex), (GLvoid*) offsetof(PTNVertex, normal)  );
	glVertexAttribPointer(DEFAULT_VERTEX_TANGENT_BINDING, 
		3, GL_FLOAT, false, 
		sizeof(PTNVertex), (GLvoid*) offsetof(PTNVertex, tangent)  );
	glVertexAttribPointer(DEFAULT_VERTEX_BITANGENT_BINDING, 
		3, GL_FLOAT, false, 
		sizeof(PTNVertex), (GLvoid*) offsetof(PTNVertex, bitangent)  );

	glEnableVertexAttribArray(DEFAULT_VERTEX_POS_BINDING);
	glEnableVertexAttribArray(DEFAULT_VERTEX_TEX_BINDING);
	glEnableVertexAttribArray(DEFAULT_VERTEX_NORMAL_BINDING);
	glEnableVertexAttribArray(DEFAULT_VERTEX_TANGENT_BINDING);
	glEnableVertexAttribArray(DEFAULT_VERTEX_BITANGENT_BINDING);

	// Build Index Map
	for(int n = 0; n < loader.GetNumElements(); n++) {
		__InternalModelGroupInfo *elm = new __InternalModelGroupInfo();
		elm->offset = loader.GetElement(n)->GetFirstIndex();
		elm->count  = loader.GetElement(n)->GetNumIndices();
		elm->hasColor = false;
		m_modelgroups[loader.GetElement(n)->GetName()] = elm;
	}

	UNBIND_MODELBUFFERS();

	glCheckandPrintError();

	return true;
}

Model::ModelGroupIterator Model::GroupsBegin()
{
	return m_modelgroups.begin();
}

Model::ModelGroupIterator Model::GroupsEnd()
{
	return m_modelgroups.end();
}

Model::ModelGroupIterator Model::findGroup(const std::string &name)
{
	return m_modelgroups.find(name);
}

const std::string & Model::getGroupName(const ModelGroupIterator iter)
{
	return iter->first;
}

bool Model::setGroupColor(const ModelGroupIterator &group_iter, const Vector &color)
{
	if(group_iter == GroupsEnd())
		return false;

	group_iter->second->color = color;
	group_iter->second->hasColor = true;
	return true;
}

const Vector& Model::getGroupColor(const ModelGroupIterator &group_iter)
{
	if(group_iter == GroupsEnd())
		return m_default_color;

	if(!group_iter->second->hasColor)
		return m_default_color;

	return group_iter->second->color;
}

bool Model::drawGroup(const ModelGroupIterator &group_iter)
{
	BIND_MODELBUFFERS();
	
	if(BIND_MODELBUFFERS_HASCHANGED()) {
		glVertexAttribPointer(DEFAULT_VERTEX_POS_BINDING, 
								3, GL_FLOAT, false, 
								 sizeof(PTNVertex),(GLvoid*) offsetof(PTNVertex, pos)  );
		glVertexAttribPointer(DEFAULT_VERTEX_TEX_BINDING, 
								2, GL_FLOAT, false, 
								sizeof(PTNVertex), (GLvoid*) offsetof(PTNVertex, texCoords)  );
		glVertexAttribPointer(DEFAULT_VERTEX_NORMAL_BINDING, 
								3, GL_FLOAT, false, 
								sizeof(PTNVertex), (GLvoid*) offsetof(PTNVertex, normal)  );
		glVertexAttribPointer(DEFAULT_VERTEX_TANGENT_BINDING, 
								3, GL_FLOAT, false, 
								sizeof(PTNVertex), (GLvoid*) offsetof(PTNVertex, tangent)  );
		glVertexAttribPointer(DEFAULT_VERTEX_BITANGENT_BINDING, 
								3, GL_FLOAT, false, 
								sizeof(PTNVertex), (GLvoid*) offsetof(PTNVertex, bitangent)  );

		glEnableVertexAttribArray(DEFAULT_VERTEX_POS_BINDING);
		glEnableVertexAttribArray(DEFAULT_VERTEX_TEX_BINDING);
		glEnableVertexAttribArray(DEFAULT_VERTEX_NORMAL_BINDING);
		glEnableVertexAttribArray(DEFAULT_VERTEX_TANGENT_BINDING);
		glEnableVertexAttribArray(DEFAULT_VERTEX_BITANGENT_BINDING);
	}
	
	if(group_iter == GroupsEnd())
		return false;


	glDrawElements(GL_TRIANGLES, 
					group_iter->second->count, 
					GL_UNSIGNED_INT,
					(GLvoid*)(group_iter->second->offset *sizeof(int)) );

	//intentionally commented to reduce buffer changes 
	//UNBIND_MODELBUFFERS();

	return true;
}

void Model::draw(World &world, DrawPass pass)
{
	//Enable depth test
	glEnable(GL_DEPTH_TEST);

	//Use Default Shader	
	ColoredPhongMaterial *material = dynamic_cast<ColoredPhongMaterial*>(world.getMaterial("ColoredPhong"));

	assert(material != NULL);
	if(material == NULL)
		return;

	material->setColor(m_default_color);
	material->setDiffuseSpecCoef(0.8f, 0.4f, 0.2f);

	material->use(getModelMatrix(), world);

	ModelGroupIterator iter = GroupsBegin();
	ModelGroupIterator end = GroupsEnd();
	for(; iter != end; iter++) {
		material->setColor(getGroupColor(iter));
		drawGroup(iter);
	}

	material->unuse();
}
