#ifndef __DrawableModel_h__
#define __DrawableModel_h__

#include "Matrix.h"
#include "Vector.h"

class World;

enum DrawPass {
	DRAWPASS_FINAL			= 0x01,
	DRAWPASS_DEPTHONLY		= 0x02,
	DRAWPASS_SHADOWMAP		= 0x04,
	DRAWPASS_PREDRAW		= 0x08,
	DRAWPASS_POSTDRAW       = 0x10
};

// Base class for things that can be drawn
class DrawableModel
{
public:
	DrawableModel() {
		m_modelMatrix = Matrix::createIdentityMatrix();
	}

	virtual ~DrawableModel() {}

	// Returns the matrix transforming points from model coordinates to world coordinates
	const Matrix &getModelMatrix() const { return m_modelMatrix; }

	// Defines how points in model coordinates are transformed to world coordinates
	void setModelMatrix(const Matrix &val) { m_modelMatrix = val; }

	// Draws the object to the given world
	virtual void draw(World & World, DrawPass pass) = 0;

protected:
	// Matrix transforming points from model coordinates to world coordinates
	Matrix m_modelMatrix;

private:
};

#endif // __DrawableModel_h__
