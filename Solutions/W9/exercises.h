#ifndef __exercises_h__
#define __exercises_h__

#include <iostream>

// Empty Skeleton
//#define EXERCISE 0

// Exercise 1:  Matrix
// finish Matrix Class, fill in empty functions
//#define EXERCISE  1

// Exercise 2:  Camera
// Use lookAtMatrix to implement Simple camera movements/
//#define EXERCISE  2

// Exercise 3:  Helicopter
// Use Matrices to rotate and position rotors. scale helicopter 
// place helicopter to roof 
//#define EXERCISE  3

// Exercise 4:  Quaternion
//Finish quaternion class, fill in empty functions
//#define EXERCISE  4

// Exercise 5:  Camera Path
// Use built in cameraviews matrices to reconstruct camera path key points
// (position and rotation quaternions), use given spline functions to interpolate 
// between key frames(position and quaternions)
#define EXERCISE  5



#define TODO(A)					\
	if((A) <= EXERCISE)			\
		std::cout << "You have to fill in Code at" << __FILE__ << ":" << __LINE__ << " for Ex.:" << #A << std::endl

#endif // __exercises_h__
