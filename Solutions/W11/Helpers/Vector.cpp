#include "Vector.h"

#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

Vector::operator const std::string()
{
	std::stringstream out;
	out << fixed << std::setprecision(3) 
		<< "{"
		<< m_elements[0] << "f," << setw(12)
		<< m_elements[1] << "f," << setw(12)
		<< m_elements[2] << "f," << setw(12)
		<< m_elements[3] << "f"
		<< "}";

	return out.str();
}
