#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <cmath>
#include <string>
#include <assert.h>

// Represents a 4-vector. Can be used as a homogeneous 3-vector (x*w, y*w, z*w, w), or a general 4-vector (x, y, z, w)
class Vector
{
public:
	//=======================================================================
	// Constructors
	//=======================================================================

	// Creates the zero 3-vector (0, 0, 0, 1)
	Vector()  
	{
		m_elements[0] = 0;
		m_elements[1] = 0;
		m_elements[2] = 0;
		m_elements[3] = 1;

		// Other code assumes that the 'Vector' class is tightly packed and only contains the four vector values
		assert(sizeof(*this) == sizeof(float)*4);
	}

	// Creates a 4-vector from the 4 float values at ptr
	Vector(const float *ptr) 
	{
		m_elements[0] = ptr[0]; 
		m_elements[1] = ptr[1]; 
		m_elements[2] = ptr[2];
		m_elements[3] = ptr[3];
	}
	
	// Creates a 4-vector (x, y, z, w)
	Vector(float x, float y = 0.0, float z = 0.0 , float w = 1.0) {
		m_elements[0] = x; 
		m_elements[1] = y; 
		m_elements[2] = z;
		m_elements[3] = w;
	}

	~Vector() {}

	// Creates a 4-vector containing the values of 'other'
	Vector(const Vector &other)
	{
		m_elements[0] = other.m_elements[0];
		m_elements[1] = other.m_elements[1];
		m_elements[2] = other.m_elements[2];
		m_elements[3] = other.m_elements[3];
	}


	//=======================================================================
	// Special Vectors
	//=======================================================================

	// Returns the vector (0, 0, 0, 1)
	static const Vector &createZeroVector3() 
	{
		static Vector zero3(0.0, 0.0, 0.0, 1.0);
		return zero3;
	}

	// Returns the vector (0, 0, 0, 0)
	static const Vector &createZeroVector4() 
	{
		static Vector zero4(0.0, 0.0, 0.0, 0.0);
		return zero4;
	}

	//=======================================================================
	// Access Functions/Operators
	//=======================================================================

	// Returns the element-th element of the vector
	float& operator[] (int element) 
	{
		assert(element >= 0 && element <= 3); 
		return m_elements[element];
	}

	// Returns the element-th element of the vector
	const float& operator[] (int element) const
	{
		assert(element >= 0 && element <= 3); 
		return m_elements[element];
	}

	// Returns the element-th element of the vector
	float& operator() (int element) 
	{
		assert(element >= 0 && element <= 3); 
		return m_elements[element];
	}

	// Returns the element-th element of the vector
	const float& operator() (int element) const
	{
		assert(element >= 0 && element <= 3); 
		return m_elements[element];
	}

	// Returns a pointer to an array of 4 floats containing the data of this vector
	float *getArray()  
	{
		return &m_elements[0];
	}

	// Returns a pointer to an array of 4 floats containing the data of this vector
	const float *getArray() const
	{
		return &m_elements[0];
	}

	//=======================================================================
	// Mathematical Vectors
	//=======================================================================
	// 
	const Vector add3(const Vector &other) const 
	{
		if(m_elements[3] == 1.0 && other[3] == 1.0) {
			return Vector(	m_elements[0] + other[0],
						    m_elements[1] + other[1],
							m_elements[2] + other[2],
							1.0f);
		} else {
			Vector t = this->homogenized();
			Vector o = other.homogenized();
			return Vector ( t[0] + o[0],
							t[1] + o[1],
							t[2] + o[2],
							1.0f);
		}
	}

	const Vector add4(const Vector &other) const
	{
		return Vector(	m_elements[0] + other[0],
						m_elements[1] + other[1],
						m_elements[2] + other[2],
						m_elements[3] + other[3]);
	}
	
	const Vector sub3(const Vector &other) const 
	{
		if(m_elements[3] == 1.0 && other[3] == 1.0) {
			return Vector(	m_elements[0] - other[0],
						    m_elements[1] - other[1],
							m_elements[2] - other[2],
							1.0f);
		} else {
			Vector t = this->homogenized();
			Vector o = other.homogenized();
			return Vector ( t[0] - o[0],
							t[1] - o[1],
							t[2] - o[2],
							1.0f);
		}
	}

	const Vector sub4(const Vector &other) const
	{
		return Vector(	m_elements[0] - other[0],
						m_elements[1] - other[1],
						m_elements[2] - other[2],
						m_elements[3] - other[3]);
	}

	const Vector multScalar3(const float other) const 
	{
		return Vector ( m_elements[0] * other,
						m_elements[1] * other,
						m_elements[2] * other,
						m_elements[3]);
	}

	const Vector multScalar4(const float other) const 
	{
		return Vector(	m_elements[0] * other,
						m_elements[1] * other,
						m_elements[2] * other,
						m_elements[3] * other);
	}

	const Vector minus3() const 
	{
		return Vector(	-m_elements[0],
						-m_elements[1],
						-m_elements[2],
						 m_elements[3]);
	}

	const Vector minus4() const{
		return Vector(	-m_elements[0],
						-m_elements[1],
						-m_elements[2],
						-m_elements[3]);
	}

	const float dot3(const Vector &other) const 
	{
		if(m_elements[3] == 1.0 && other[3] == 1.0) {
			return	m_elements[0] * other[0] +
					m_elements[1] * other[1] +
					m_elements[2] * other[2];
		} else {
			Vector t = this->homogenized();
			Vector o = other.homogenized();
			return	t[0] * o[0] +
					t[1] * o[1] +
					t[2] * o[2];
		}
	}

	const float dot4(const Vector &other) const 
	{
		return		m_elements[0] * other[0] +
					m_elements[1] * other[1] +
					m_elements[2] * other[2] +
					m_elements[3] * other[3];
	}

	const Vector cross3(const Vector &other) const 
	{
		if(m_elements[3] == 1.0 && other[3] == 1.0) {
			return Vector(	 m_elements[1] * other[2] - m_elements[2] * other[1],
						    -m_elements[0] * other[2] + m_elements[2] * other[0],
							 m_elements[0] * other[1] - m_elements[1] * other[0],
							1.0f);
		} else {
			Vector t = this->homogenized();
			Vector o = other.homogenized();
			return Vector (  t[1] * o[2] - t[2] * o[1],
						    -t[0] * o[2] + t[2] * o[0],
							 t[0] * o[1] - t[1] * o[0],
							1.0f);
		}
	}

	const float sqrdLength3() const 
	{
		return this->dot3(*this);
	}

	const float sqrdLength4() const 
	{
		return this->dot4(*this);
	}
	
	const float length3() const 
	{
		return sqrt(this->sqrdLength3());
	}

	const float length4() const 
	{
		return sqrt(this->sqrdLength4());
	}

	const Vector normalized3() const {
		return *this * (1.0f / length3());
	}

	const Vector normalized4() const {
		return this->multScalar4(1.0f / length4());
	}

	const Vector homogenized() const {
		if(m_elements[3] == 1.0f) {
			return *this;
		} else { 
			float invw = m_elements[3]==0.0f ? 1.0f : 1.0f/m_elements[3];
			return Vector(m_elements[0]*invw, m_elements[1]*invw, m_elements[2]*invw, 1.0);
		}
	};

	const Vector operator+(const Vector &other) const { return add3(other); }
	const Vector operator-(const Vector &other) const { return sub3(other); }
	const Vector operator*(const Vector &other) const { return dot3(other); }
	const Vector operator*(const float other) const { return multScalar3(other); }
	const Vector operator^(const Vector &other) const { return cross3(other); }
	const Vector operator-() const { return minus3(); }

	
	//=======================================================================
	// =, == operators
	//=======================================================================
	const Vector& operator= (const Vector &other) 
	{
		m_elements[0] = other.m_elements[0];
		m_elements[1] = other.m_elements[1];
		m_elements[2] = other.m_elements[2];
		m_elements[3] = other.m_elements[3];
		return *this;
	}
	
	const Vector& operator+= (const Vector &other) { return *this = *this + other; }
	const Vector& operator-= (const Vector &other) { return *this = *this - other; }
	const Vector& operator*= (const Vector &other) { return *this = *this * other; }
	const Vector& operator*= (const float &other) { return *this = *this * other; }
	const Vector& operator^= (const Vector &other) { return *this = *this ^ other; }

	bool operator== (const Vector &other) 
	{
		if(m_elements[3] == 1.0 && other[3] == 1.0) {
				return	m_elements[0] == other[0] &&
						m_elements[1] == other[1] &&
						m_elements[2] == other[2];
			} else {
				Vector t = this->homogenized();
				Vector o = other.homogenized();
				return	t[0] == o[0] &&
						t[1] == o[1] &&
						t[2] == o[2];
			}
	}

	bool operator!= (const Vector &other)
	{
		return !((*this) == other);
	}

	operator const std::string ();

private: // you should not add any other class members! Matrix class relies on this
	float m_elements[4];
};

#endif
