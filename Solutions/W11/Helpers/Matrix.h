#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <string.h>
#include "Vector.h"

const float PI = 3.141592653589793238462f;

class Matrix 
{
public:
	// Converts degrees (in 0 - 360) to radians (in 0 - 2*PI)
	static float radians(float degree) { return degree/180.0f*PI; }

	//=======================================================================
	// Constructors
	//=======================================================================
	// Creates a zero matrix
	Matrix()
	{
		setZero();
	}

	// Creates a matrix from the 16 values at 'ptr'
	Matrix(const float *ptr)
	{
		memcpy(getCArray(), ptr, 16*sizeof(float));
	}
	
	// Creates a matrix with 'diagonal' on the diagonal and 0 everywhere else
	Matrix(const Vector &diagonal) 
	{
		setZero();

		m_rows[0][0] = diagonal[0];
		m_rows[1][1] = diagonal[1];
		m_rows[2][2] = diagonal[2];
		m_rows[3][3] = diagonal[3];
	}

	// Creates a matrix from four row vectors or column vectors (depending on the value of 'isColumnVectors')
	Matrix(const Vector &v0, const Vector &v1, const Vector &v2, const Vector &v3, bool isColumnVectors = false)
	{
		if(isColumnVectors == false) {
			m_rows[0] = v0;
			m_rows[1] = v1;
			m_rows[2] = v2;
			m_rows[3] = v3;
		} else {
			m_rows[0][0]=v0[0];m_rows[0][1]=v1[0];m_rows[0][2]=v2[0];m_rows[0][3]=v3[0];
			m_rows[1][0]=v0[1];m_rows[1][1]=v1[1];m_rows[1][2]=v2[1];m_rows[1][3]=v3[1];
			m_rows[2][0]=v0[2];m_rows[2][1]=v1[2];m_rows[2][2]=v2[2];m_rows[2][3]=v3[2];
			m_rows[3][0]=v0[3];m_rows[3][1]=v1[3];m_rows[3][2]=v2[3];m_rows[3][3]=v3[3];
		}
	}

	// Creates a matrix containing the values of the matrix 'other'
	Matrix(const Matrix& other)
	{
		m_rows[0] = other.m_rows[0];
		m_rows[1] = other.m_rows[1];
		m_rows[2] = other.m_rows[2];
		m_rows[3] = other.m_rows[3];
	}

	// Destructor
	~Matrix() {}

	//=======================================================================
	// Special Matrices
	//=======================================================================
	// Returns the identity matrix
	static const Matrix &createIdentityMatrix();

	// Returns the all-zero matrix
	static const Matrix &createZeroMatrix();

	static const Matrix createOrthoMatrix(float left, float right, float bottom, float top, float zNear, float zFar);
	
	static const Matrix createPerspectiveMatrix(float fovy, float aspect, float zNear, float zFar);

	static const Matrix createLookAtMatrix(const Vector &eyePos, const Vector &lookAtPos, const Vector &upVec);

	static const Matrix createRotationXMatrix(float angle_radian);
	static const Matrix createRotationYMatrix(float angle_radian);
	static const Matrix createRotationZMatrix(float angle_radian);
	static const Matrix createRotationMatrix(const Vector& axis, float angle_radian);

	// Returns a matrix that represents a translation by 'pos'
	static const Matrix createTranslationMatrix ( const Vector& pos );

	// Returns a matrix that represents scaling by sx in x direction, sy in y direction, and sz in z direction
	static Matrix const createScaleMatrix(float sx, float sy, float sz);

	//=======================================================================
	// Access Functions/Operators
	//=======================================================================
	// Returns the row-th row of the matrix
	Vector& operator[] (int row) 
	{
		assert(row >= 0 && row <= 3); 
		return m_rows[row];
	}

	// Returns the row-th row of the matrix
	const Vector& operator[] (int row) const
	{
		assert(row >= 0 && row <= 3); 
		return m_rows[row];
	}

	// Returns the element M(row, col) of the matrix
	float& operator() (int row, int col) 
	{
		assert(row >= 0 && row <= 3); 
		return m_rows[row][col];
	}

	// Returns the element M(row, col) of the matrix
	const float& operator() (int row, int col) const
	{
		assert(row >= 0 && row <= 3); 
		return m_rows[row][col];
	}

	// Returns a pointer to the 16 floats representing the data of this matrix. The values
	// are stored in the order M(0, 0), M(0, 1), M(0, 2), etc...
	float *getCArray()  
	{
		//be sure that vectors are consecutive in memory
		assert( &(m_rows[0][0]) + 4 == &(m_rows[1][0]) &&
				&(m_rows[1][0]) + 4 == &(m_rows[2][0]) &&
				&(m_rows[2][0]) + 4 == &(m_rows[3][0])
			   );

		return &(m_rows[0][0]);
	}

	// Returns a pointer to the 16 floats representing the data of this matrix. The values
	// are stored in the order M(0, 0), M(0, 1), M(0, 2), etc...
	const float *getCArray() const
	{
		//be sure that vectors are consecutive in memory
		assert( &(m_rows[0][0]) + 4 == &(m_rows[1][0]) &&
				&(m_rows[1][0]) + 4 == &(m_rows[2][0]) &&
				&(m_rows[2][0]) + 4 == &(m_rows[3][0])
			   );

		return &(m_rows[0][0]);
	}

	const Matrix getNoTranslationMatrix() const {
		Matrix mat = *this;
		mat(0, 3) = 0;
		mat(1, 3) = 0;
		mat(2, 3) = 0;

		return mat;
	}
	
	//=======================================================================
	// Mathematical Operators
	//=======================================================================
	const Matrix operator+(const Matrix &other) const
	{
		return Matrix(	m_rows[0].add4(other.m_rows[0]),
						m_rows[1].add4(other.m_rows[1]),
						m_rows[2].add4(other.m_rows[2]),
						m_rows[3].add4(other.m_rows[3])
					 );
	}

	const Matrix operator-(const Matrix &other) const
	{
		return Matrix(	m_rows[0].sub4(other.m_rows[0]),
						m_rows[1].sub4(other.m_rows[1]),
						m_rows[2].sub4(other.m_rows[2]),
						m_rows[3].sub4(other.m_rows[3])
					 );
	}
	
	const Matrix operator*(const Matrix &other) const
	{
		Matrix result;

		for(int r = 0; r < 4; r++)
			for(int c = 0; c < 4; c++)
				result[r][c] = m_rows[r].dot4( 
									Vector(other[0][c], other[1][c], other[2][c], other[3][c]) 
							   );
		return result;
	
	}

	const Vector operator*(const Vector &other) const
	{
		return Vector(  m_rows[0].dot4(other),
						m_rows[1].dot4(other),
						m_rows[2].dot4(other),
						m_rows[3].dot4(other)
					 );

	}

	const Matrix operator*(const float other) const
	{
		return Matrix(	m_rows[0].multScalar4(other),
						m_rows[1].multScalar4(other),
						m_rows[2].multScalar4(other),
						m_rows[3].multScalar4(other)
					 );
	}
	
	// Returns the inverse M^{-1} of this matrix
	const Matrix inversed() const;

	const Matrix normaltransform() const;

	// Returns the transpose M^T of this matrix
	const Matrix transposed() const
	{
		return Matrix(	m_rows[0],
						m_rows[1],
						m_rows[2],
						m_rows[3],
						true       //transpose
					 );
	}

	// Returns true iff the matrix is orthogonal, i.e. M^T * M = M * M^T = I
	const bool isOrthogonal() const;

	// Returns an orthogonalized copy of this matrix
	const Matrix orthogonalize() const;
	
	// Returns the determinant of the matrix
	float determinant() const;

	// Returns the cofactor C_{r, c} of the matrix
	float cofactor(int r, int c) const;

	// Sets all elements of the matrix to zero
	void setZero() {
		memset(getCArray(), 0, 16*sizeof(float));
	}

	//=======================================================================
	// =, == operators
	//=======================================================================
	const Matrix& operator=(const Matrix &other)
	{
		m_rows[0] = other.m_rows[0];
		m_rows[1] = other.m_rows[1];
		m_rows[2] = other.m_rows[2];
		m_rows[3] = other.m_rows[3];
		return *this;
	}
	
	const Matrix& operator+=(const Matrix &other) { return *this = *this + other; }
	const Matrix& operator-=(const Matrix &other) { return *this = *this - other; }
	const Matrix& operator*=(const Matrix &other) { return *this = *this * other; }
	const Matrix& operator*=(const float &other) { return *this = *this * other; }

	const bool operator== (const Vector &other) 
	{
		return	m_rows[0] == other[0] && 
				m_rows[1] == other[1] &&
				m_rows[2] == other[2] &&
				m_rows[3] == other[3];
	}

	// Returns a textual representation of this matrix
	operator const std::string ();
	
private:
	// The four rows of the 4x4 matrix
	Vector m_rows[4];
};

#endif
