#include "ObjectPTN.h"
#include <math.h>

#define PTN_ELEMENT_MAGICNUM 'PTNE'

#define PTN_OBJECT_MAGICNUM 'PTN2'

#define MAX_ELEMENT_NAME_LEN 1024

bool ElementPTN::LoadFromStream(ifstream &stream)
{
	unsigned int magic;
	stream.read((char*)&magic, sizeof(magic));

	if(magic != PTN_ELEMENT_MAGICNUM)
		return false;

	// Read name:
	string name = "";
	char ch = 0;
	stream.read(&ch, sizeof(ch));
	while(ch != 0)
	{
		if(name.size() > MAX_ELEMENT_NAME_LEN)
			return false;

		name += ch;
		stream.read(&ch, sizeof(ch));
	}

	m_name = name;

	// Read first index:
	stream.read((char*)&m_firstTriIndex, sizeof(m_firstTriIndex));

	// Read num indices:
	stream.read((char*)&m_numTriIndices, sizeof(m_numTriIndices));

	return true;
}

void ElementPTN::WriteToStream(ofstream &stream)
{
	unsigned int magic = PTN_ELEMENT_MAGICNUM;
	stream.write((const char*)&magic, sizeof(magic));

	// Limit size of name:
	string name = m_name.substr(0, MAX_ELEMENT_NAME_LEN);

	// Write name:
	stream.write(name.c_str(), name.size());
	char zero = 0;
	stream.write(&zero, sizeof(zero));

	// Write first index:
	stream.write((const char*)&m_firstTriIndex, sizeof(m_firstTriIndex));

	// Write num indices:
	stream.write((const char*)&m_numTriIndices, sizeof(m_numTriIndices));
}


ObjectPTN::ObjectPTN()
{
	assert(sizeof(m_vertices[0]) == 4*3 + 4*2 + 4*3 + 4*3 + 4*3);
}

bool ObjectPTN::LoadFromFile(const string &filename)
{
	deleteAllElements();

	ifstream stream;
	stream.open(filename.c_str(), ios::binary);

	if(!stream)
		return false;

	unsigned int magic;
	stream.read((char*)&magic, sizeof(magic));

	if(magic != PTN_OBJECT_MAGICNUM)
		return false;

	// Read vertices:
	unsigned int numVtx;
	stream.read((char*)&numVtx, sizeof(numVtx));

	m_vertices.resize(numVtx);
	stream.read((char*)&m_vertices[0], sizeof(m_vertices[0]) * numVtx);

	// Read triangle indices:
	unsigned int numTris;
	stream.read((char*)&numTris, sizeof(numTris));

	m_triIndices.resize(numTris*3);
	stream.read((char*)&m_triIndices[0], sizeof(m_triIndices[0]) * numTris * 3);


	// Read elements:
	unsigned int numEls;
	stream.read((char*)&numEls, sizeof(numEls));

	m_elements.resize(numEls, NULL);
	for(unsigned int i=0; i<numEls; i++)
	{
		m_elements[i] = new ElementPTN();
		if(!m_elements[i]->LoadFromStream(stream))
		{
			deleteAllElements();
			return false;
		}
	}

	return true;
}

bool ObjectPTN::WriteToFile(const string &filename)
{
	ofstream stream;
	stream.open(filename.c_str(), ios::binary);

	if(!stream)
		return false;

	unsigned int magic = PTN_OBJECT_MAGICNUM;
	stream.write((const char*)&magic, sizeof(magic));

	assert(m_vertices.size() > 0);
	assert(m_triIndices.size() > 0);

	// Write vertices:
	unsigned int numVtx = (unsigned int)m_vertices.size();
	stream.write((const char*)&numVtx, sizeof(numVtx));

	stream.write((const char*)&m_vertices[0], sizeof(m_vertices[0]) * m_vertices.size());


	// Write triangle indices:
	assert(m_triIndices.size() % 3 == 0);
	unsigned int numTris = (unsigned int)m_triIndices.size() / 3;
	stream.write((const char*)&numTris, sizeof(numTris));

	stream.write((const char*)&m_triIndices[0], sizeof(m_triIndices[0]) * m_triIndices.size());

	// Write elements:
	unsigned int numEls = GetNumElements();
	stream.write((const char*)&numEls, sizeof(numEls));

	for(int i=0; i<GetNumElements(); i++)
	{
		m_elements[i]->WriteToStream(stream);
	}

	return true;
}

void ObjectPTN::ComputeTangents()
{
	assert(m_triIndices.size() % 3 == 0);
	int numTri = (int)m_triIndices.size() / 3;

	int numVtx = (int)m_vertices.size();

	// Clear tangents and bitangents:
	for(int vtxIdx=0; vtxIdx<numVtx; vtxIdx++)
	{
		for(int dim=0; dim<3; dim++)
			m_vertices[vtxIdx].tangent[dim] = 0;

		for(int dim=0; dim<3; dim++)
			m_vertices[vtxIdx].bitangent[dim] = 0;
	}

	for(int triIdx=0; triIdx<numTri; triIdx++)
	{
		// Get triangle indices:
		int idx[3];
		for(int dim=0; dim<3; dim++)
			idx[dim] = m_triIndices[triIdx*3+dim];

		// Get vertex positions:
		const float *p0 = m_vertices[idx[0]].pos;
		const float *p1 = m_vertices[idx[1]].pos;
		const float *p2 = m_vertices[idx[2]].pos;

		// a = p1 - p0:
		float a[3];
		for(int dim=0; dim<3; dim++)
			a[dim] = p1[dim] - p0[dim];

		// b = p2 - p0:
		float b[3];
		for(int dim=0; dim<3; dim++)
			b[dim] = p2[dim] - p0[dim];

		// Get vertex texture coordinates:
		const float *t0 = m_vertices[idx[0]].texCoords;
		const float *t1 = m_vertices[idx[1]].texCoords;
		const float *t2 = m_vertices[idx[2]].texCoords;

		// ta = t1 - t0
		float ta[2];
		for(int i=0; i<2; i++)
			ta[i] = t1[i] - t0[i];

		// tb = t2 - t0
		float tb[2];
		for(int i=0; i<2; i++)
			tb[i] = t2[i] - t0[i];

		/*
		Vectors a and b are two triangle edges.
		Vectors ta and tb are the corresponding vectors in texture coordinates.
		In texture coordinates, the tangent vector is (1, 0)^T and the bitangent is (0, 1)^T.
		We are looking for the corresponding tangent (T) and bitangent (B) vectors in 3D coordinates.
		In texture coordinates, we can represent the tangent and bitangent in terms of ta and tb
		using four coefficients xi_a, xi_b, eta_a and eta_b:

			(1, 0)^T = xi_a * ta + xi_b * tb
			(0, 1)^T = eta_a * ta + eta_b * tb

		On the other hand, we can represent T and B in terms of a and b using the same coefficients:

			T = xi_a * a + xi_b * b
			B = eta_a * a + eta_b * b

		The first two equations can be written as:

			I_2  = [ta, tb] * Minv
			Minv = [xi_a, eta_a; xi_b, eta_b]

		Where [ta, tb] is a 2x2 matrix containing the ta and tb as columns. Consequently:

			Minv = [ta, tb]^-1

		Rewriting the equations for T and B:

			[T, B] = [a, b] * Minv

		Note that [T, B] and [a, b] are 3x2 matrices.
		*/

		// Matrix M = [ta, tb]:
		float m11 = ta[0];
		float m12 = tb[0];
		float m21 = ta[1];
		float m22 = tb[1];

		// Matrix Minv = [ta, tb]^-1
		float det = m11*m22 - m12*m21;
		float Minv11 = 0;
		float Minv12 = 0;
		float Minv21 = 0;
		float Minv22 = 0;

		if(fabs(det) > 1e-12)
		{
			Minv11 =  m22 / det;
			Minv12 = -m12 / det;
			Minv21 = -m21 / det;
			Minv22 =  m11 / det;
		}

		// tangent = a * MInv_11 + b * MInv_21;
		float tangent[3];
		for(int dim=0; dim<3; dim++)
			tangent[dim] = a[dim] * Minv11 + b[dim] * Minv21;

		// bitangent = a * MInv_12 + b * MInv_22;
		float bitangent[3];
		for(int dim=0; dim<3; dim++)
			bitangent[dim] = a[dim] * Minv12 + b[dim] * Minv22;

		// normalize tangent:
		normalizeVec3(tangent);

		// normalize bitangent:
		normalizeVec3(bitangent);

		for(int idxTriVtx=0; idxTriVtx<3; idxTriVtx++)
		{
			// Add tangent to vertex:
			for(int dim=0; dim<3; dim++)
				m_vertices[idx[idxTriVtx]].tangent[dim] += tangent[dim];

			// Add bitangent to vertex:
			for(int dim=0; dim<3; dim++)
				m_vertices[idx[idxTriVtx]].bitangent[dim] += bitangent[dim];
		}
	}

	/*
	At this point we have computed the tangents and bitangents for each face and
	summed them over adjacent vertices.
	In the next step, we project the vertex tangent and bitangent vectors to the
	plane perpendicular to the normal and then re-normalize them.
	This results in the tangent and bitangent having unit length and being normal to the normal vector.
	*/
	for(int vtxIdx=0; vtxIdx<numVtx; vtxIdx++)
	{
		// Get normal:
		float normal[3];
		for(int dim=0; dim<3; dim++)
			normal[dim] = m_vertices[vtxIdx].normal[dim];

		// Make sure normal is normalized:
		normalizeVec3(normal);

		// Get tangent:
		float *tangent = m_vertices[vtxIdx].tangent;

		// tangent = tangent - normal * (normal DOT tangent);
		float dotProd = dot3(normal, tangent);
		for(int dim=0; dim<3; dim++)
			tangent[dim] -= normal[dim] * dotProd;

		normalizeVec3(tangent);

		// Get bitangent:
		float *bitangent = m_vertices[vtxIdx].bitangent;

		// bitangent = bitangent - normal * (normal DOT bitangent)
		dotProd = dot3(normal, bitangent);
		for(int dim=0; dim<3; dim++)
			bitangent[dim] -= normal[dim] * dotProd;

		normalizeVec3(bitangent);
	}
}

void ObjectPTN::normalizeVec3(float *pVec)
{
	float norm = sqrt(pVec[0]*pVec[0] + pVec[1]*pVec[1] + pVec[2]*pVec[2]);

	float normInv = 0;
	if(fabs(norm) > 1e-12)
		normInv = 1.0f / norm;

	for(int dim=0; dim<3; dim++)
		pVec[dim] *= normInv;
}

float ObjectPTN::dot3(float *pA, float *pB)
{
	float dotProd = 0;

	for(int dim=0; dim<3; dim++)
		dotProd += pA[dim] * pB[dim];

	return dotProd;
}
