#include "Matrix.h"

#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

const bool Matrix::isOrthogonal() const
{
	const float eps = 1.0e-15f;

	//check if rows are orthogonal
	if( abs( m_rows[0].dot4(m_rows[1]) ) > eps)
		return false;
	if( abs( m_rows[0].dot4(m_rows[2]) ) > eps)
		return false;
	if( abs( m_rows[0].dot4(m_rows[3]) ) > eps)
		return false;
	if( abs( m_rows[1].dot4(m_rows[2]) ) > eps)
		return false;
	if( abs( m_rows[1].dot4(m_rows[3]) ) > eps)
		return false;
	if( abs( m_rows[2].dot4(m_rows[3]) ) > eps)
		return false;
	//normalized ?
	if( abs(abs( m_rows[0].dot4(m_rows[0]) )-1) > eps)
		return false;
	if( abs(abs( m_rows[1].dot4(m_rows[1]) )-1) > eps)
		return false;
	if( abs(abs( m_rows[2].dot4(m_rows[2]) )-1) > eps)
		return false;
	if( abs(abs( m_rows[3].dot4(m_rows[3]) )-1) > eps)
		return false;

	return true;
}
	
const Matrix Matrix::orthogonalize() const
{
	//stabilized  Gram-Schmidt
	//http://en.wikipedia.org/wiki/Gram%E2%80%93Schmidt_process
	Matrix v(*this);

	for(int j = 0; j <= 3; j++) {
		for (int i = 0; i <= j - 1; i++) {
			v[j] = v[j].sub4(v[i].multScalar4(( v[i].dot4(v[j]) / v[i].sqrdLength4() )));
		}
		v[j] = v[j].normalized4();
	}

	return v;
}



const Matrix Matrix::normaltransform() const
{
	Matrix transposed_adjoint;
	
	for(int r = 0; r < 4; r++) 
		for(int c = 0; c < 4; c++) 
			transposed_adjoint[r][c] = this->cofactor(r,c); 

	return transposed_adjoint;
}


const Matrix Matrix::inversed() const
{
	//slow straight forward implementation
	//may replace by Block Matrix Inversion
	//as shown in Numerical Recipes
	const float eps = 1.0e-30f;

	// use this if if you often inverse a orthonormal (rotation) matrix
	if( this->isOrthogonal() )
		return this->transposed();

	Matrix inv;

	// now do the general inversion as learned in school
	// => replacing all elements by its cofactors, transpose and
	// divide by main determinant
	for(int r = 0; r < 4; r++) 
		for(int c = 0; c < 4; c++) 
			inv[r][c] = this->cofactor(c,r);  //do transpose in this step as well

	//calculate determinate along first row (inv is transposed!)
	float main_det = inv[0][0] * m_rows[0][0]  + inv[1][0] * m_rows[0][1] + inv[2][0] * m_rows[0][2] + inv[3][0] * m_rows[0][3];

	if( abs(main_det) < eps) {
		assert(!"You now you shouldn't inverse singular matrices, do you?");
		return Matrix();
	}

	//divide by determinant
	return inv * (1/main_det);
}

float Matrix::determinant() const
{
	//calculate determinate along 4th row
	//which most likely contains some zeros
	return	(m_rows[3][0]==0.0f)?0.0f:cofactor(3,0) * m_rows[3][0] + 
			(m_rows[3][1]==0.0f)?0.0f:cofactor(3,1) * m_rows[3][1] + 
			(m_rows[3][2]==0.0f)?0.0f:cofactor(3,2) * m_rows[3][2] + 
			(m_rows[3][3]==0.0f)?0.0f:cofactor(3,3) * m_rows[3][3];
}

float Matrix::cofactor(int row, int col) const
{
	//the sign is of cofactors is alternating
	float sign = ((row+col) % 2 == 0)? 1.0f: -1.0f;

#define m(r,c) (m_rows[(r)>=row?(r)+1:(r)][(c)>=col?(c)+1:(c)])
	return sign *	(	 m(0,0)*m(1,1)*m(2,2) + m(0,1)*m(1,2)*m(2,0) + m(0,2)*m(1,0)*m(2,1)
						-m(2,0)*m(1,1)*m(0,2) - m(2,1)*m(1,2)*m(0,0) - m(2,2)*m(1,0)*m(0,1)
					);
#undef m
}



const Matrix &Matrix::createIdentityMatrix()
{
	static Matrix matId(Vector(1.0,1.0,1.0,1.0));
	return matId;
}

const Matrix &Matrix::createZeroMatrix()
{
	static Matrix matZero;
	return matZero;
}


const Matrix Matrix::createOrthoMatrix(float left, float right, float bottom, float top, float zNear, float zFar)
{
	float tx = - (right+left)/(right-left);
	float ty = - (top+bottom)/(top-bottom);
	float tz = - (zFar+zNear)/(zFar-zNear);

	return Matrix(	
		Vector(	2/(right-left),	0.0,			0.0,				tx),
		Vector(	0.0,			2/(top-bottom),	0.0,				ty),
		Vector( 0.0,			0.0,			-2/(zFar-zNear),	tz),
		Vector(	0.0,			0.0,			0.0,				1.0)		
		);
}

const Matrix Matrix::createPerspectiveMatrix(float fovy, float aspect, float zNear, float zFar)
{
	float f = 1.0f / tan(fovy/2);

	return Matrix(	
		Vector(	f/aspect,		0.0,			0.0,						0.0),
		Vector(	0.0,			f,				0.0,						0.0),
		Vector( 0.0,			0.0,			(zFar+zNear)/(zNear-zFar),	(2*zFar*zNear)/(zNear-zFar)),	
		Vector(	0.0,			0.0,			-1.0,						0.0)		
	);
}

const Matrix Matrix::createLookAtMatrix(const Vector &eyePos, const Vector &lookAtPos, const Vector &upVec)
{
	const Vector F = lookAtPos - eyePos;
	Vector f = F.normalized3();
	const Vector up = upVec.normalized3();
	Vector s = f.cross3(up).normalized3();
	Vector u = s.cross3(f);
	s[3] = 0;
	u[3] = 0;
	f[3] = 0;

	Matrix M =  Matrix(	 s,
						u,
						-f,
						Vector(0.0	, 0.0	, 0.0	, 1.0)		
						);

	M = M * createTranslationMatrix(-eyePos);

	return M;
}

const Matrix  Matrix::createRotationXMatrix(float angle_radian)
{
	float c = cos(angle_radian);
	float s = sin(angle_radian);

	return Matrix(	Vector(1.0	, 0.0	, 0.0	, 0.0),
					Vector(0.0	, c		, s		, 0.0),
					Vector(0.0	, -s	, c		, 0.0),
					Vector(0.0	, 0.0	, 0.0	, 1.0)		
				 );
}

const Matrix  Matrix::createRotationYMatrix(float angle_radian)
{
	float c = cos(angle_radian);
	float s = sin(angle_radian);

	return Matrix(	Vector(c 	, 0.0 	, -s	, 0.0),
					Vector(0.0	, 1.0	, 0.0	, 0.0),
					Vector(s 	, 0.0	, c		, 0.0),
					Vector(0.0	, 0.0	, 0.0	, 1.0)		
				 );
}

const Matrix  Matrix::createRotationZMatrix(float angle_radian)
{
	float c = cos(angle_radian);
	float s = sin(angle_radian);

	return Matrix(	Vector(c 	, s 	, 0.0	, 0.0),
					Vector(-s	, c		, 0.0	, 0.0),
					Vector(0.0 	, 0.0	, 1.0	, 0.0),
					Vector(0.0	, 0.0	, 0.0	, 1.0)		
				);
}

const Matrix  Matrix::createRotationMatrix(const Vector &axis, float angle_radian)
{
	Vector l = axis.homogenized().normalized3();
	float c = cos(angle_radian);
	float s = sin(angle_radian);
	float C = 1-c;
	float x=l[0], y=l[1], z=l[2];
	float xs = x*s,   ys = y*s,   zs = z*s;
	float xC = x*C,   yC = y*C,   zC = z*C;
	float xyC = x*yC, yzC = y*zC, zxC = z*xC;
	
	return Matrix(	Vector(	x*xC+c,		xyC-zs,		zxC+ys,		0),
					Vector(	xyC+zs,		y*yC+c,		yzC-xs,		0),
					Vector( zxC-ys,		yzC+xs,		z*zC+c,		0),
					Vector(	0.0,		0.0,		0.0,		1.0)		
					);
}

const Matrix  Matrix::createTranslationMatrix(const Vector &pos)
{
	return Matrix(	Vector(1.0 	, 0.0 	, 0.0	, pos[0]),
					Vector(0.0	, 1.0	, 0.0	, pos[1]),
					Vector(0.0 	, 0.0	, 1.0	, pos[2]),
					Vector(0.0	, 0.0	, 0.0	, 1.0)		
					);
}

Matrix const Matrix::createScaleMatrix(float sx, float sy, float sz)
{
	return Matrix(Vector(sx,sy,sz, 1.0));
}

Matrix::operator const std::string()
{
	std::stringstream out;
	out << std::setprecision(3);
	out << "{" << endl;
	out << '\t' << (const string) m_rows[0] << "," << endl;
	out << '\t' << (const string) m_rows[1] << "," << endl;
	out << '\t' << (const string) m_rows[2] << "," << endl;
	out << '\t' << (const string) m_rows[3] << " " << endl;
	out << "}";

	return out.str();
}
