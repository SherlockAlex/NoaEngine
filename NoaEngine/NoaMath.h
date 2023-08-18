#ifndef NOAENGINE_NOAMATH_H
#define NOAENGINE_NOAMATH_H

#include <math.h>

// Math Constance
#define PI 3.14159

//∂®“Â2Œ¨ ∏¡ø
template <class T>
struct Vector {
	T x;
	T y;

	Vector() 
	{

	}

	Vector(T x,T y) 
	{
		this->x = x;
		this->y = y;
	}

	Vector operator + (Vector a)
	{
		Vector result;
		result.x = x + a.x;
		result.y = y + a.y;
		return result;
	}

	Vector operator - (Vector a)
	{
		Vector result;
		result.x = x - a.x;
		result.y = y - a.y;
		return result;
	}

	T operator * (Vector a) 
	{
		T result = x * a.x + y * a.y;
		return result;
	}

};

#endif

