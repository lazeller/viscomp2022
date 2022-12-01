#ifndef OBJECT_PTN_H
#define OBJECT_PTN_H

#include <assert.h>
#include <fstream>
#include <vector>

using namespace std;

#pragma pack(push, 1)    

// A Pos/Texcoord/Normal (PTN) contains a position, a texture coordinate and a normal. And a tangent and a bitangent.
struct PTNVertex
{
	// Position of the vertex
	float pos[3];

	// Texture coordinates at the vertex
	float texCoords[2];

	// Normalized normal vector at the vertex
	float normal[3];

	// Tangent vector
	float tangent[3];

	// Bitangent vector
	float bitangent[3];
};

#pragma pack(pop)


// 3D object element with positions, texture coordinates and normals
class ElementPTN
{
public:
	ElementPTN() {}

	// Creates an element with a certain name and sets the start index and number of indices in the object's index buffer
	ElementPTN(const string &name, int firstTriIndex, int numTriIndices) {
		m_name = name;
		m_firstTriIndex = firstTriIndex;
		m_numTriIndices = numTriIndices;
	}

	virtual ~ElementPTN() {}

	// De-serializes the element from a stream, returns true iff succeeded
	bool LoadFromStream(ifstream &stream);

	// Serializes the element to a stream
	void WriteToStream(ofstream &stream);

	// Returns the name of the object
	const string &GetName() const { return m_name; }

	// Returns the index (in the object's index buffer) where indices for this object start
	int GetFirstIndex() const { return m_firstTriIndex; }

	// Returns the number of indices used by this element
	int GetNumIndices() const { return m_numTriIndices; }

protected:
	// Name of the object element
	string m_name;

	// First index in the index buffer
	int m_firstTriIndex;

	// Number of indices (multiple of 3)
	int m_numTriIndices;
};


// A collection of elements with a common vertex buffer and index buffer
class ObjectPTN
{
public:
	ObjectPTN();

	virtual ~ObjectPTN() {
		deleteAllElements();
	}

	// Loads the object from a file. Returns true iff succeeded
	bool LoadFromFile(const string &filename);

	// Save the object to a file
	bool WriteToFile(const string &filename);

	// Returns the number of elements in the file
	int GetNumElements() const { return m_elements.size(); }

	// Returns the idx-th element
	const ElementPTN *GetElement(int idx) const { return m_elements[idx]; }

	// Returns the total number of indices (equal to number of triangles times 3)
	int GetNumIndices() const { return m_triIndices.size(); }

	// Returns a pointer to a buffer containing 'GetNumIndices()' ints.
	const int *GetIndexBufferPtr() const {
		if(m_triIndices.size() == 0)
			return NULL;

		return &m_triIndices[0];
	}

	// Returns the number of vertices
	int GetNumVertices() const { return m_vertices.size(); }

	// Returns a pointer to 'GetNumVertices()' vertices
	const PTNVertex *GetVertexBufferPtr() const {
		if(m_vertices.size() == 0)
			return NULL;

		return &m_vertices[0];
	}

	void ComputeTangents();

protected:
	void deleteAllElements() {
		for(int i=0; i<(int)m_elements.size(); i++)
		{
			delete m_elements[i];
		}
		m_elements.clear();

		m_triIndices.clear();
		m_vertices.clear();
	}

private:
	static void normalizeVec3(float *pVec);

	static float dot3(float *pA, float *pB);

protected:
	// The elements that make up the object
	vector<ElementPTN*> m_elements;

	// Contains triples of vertex-indices
	vector<int> m_triIndices;

	// Contains positions, texture coordinates and normals for each vertex
	vector<PTNVertex> m_vertices;
};

#endif
