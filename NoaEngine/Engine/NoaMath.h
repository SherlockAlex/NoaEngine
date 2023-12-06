#ifndef NOAENGINE_NOAMATH_H
#define NOAENGINE_NOAMATH_H

#include <math.h>
#include <string>
#include <unordered_map>
#include <map>
#include <vector>

#include "Core.h"

namespace noa {
// Math Constance
#define PI 3.1415926f
#define HALFPI 1.57079633f
#define MUTIPI 6.28318531f
#define Min(a,b) ((a>b)?b:a)
#define Max(a,b) ((a>b)?a:b)

//∂®“Â2Œ¨ ∏¡ø
	template <class T>
	struct NOA_API Vector {
		T x;
		T y;


		Vector()
		{
			x = static_cast<T>(0);
			y = static_cast<T>(0);
		}

		Vector(T x, T y)
		{
			this->x = x;
			this->y = y;
		}

		Vector Left() {
			return Vector(-1, 0);
		}

		Vector Right() {
			return Vector(1, 0);
		}

		Vector Up() {
			return Vector(0, -1);
		}

		Vector Down() {
			return Vector(0, 1);
		}

		Vector Zero() {
			return Vector(0, 0);
		}

		Vector Normalize() const
		{

			if (this->x==0&&this->y==0)
			{
				return Vector(0, 0);
			}

			const float angle = std::atan2(this->y, this->x);
			return {std::cosf(angle),std::sinf(angle)};

		}

	
		float Magnitude() const
		{
			return sqrtf(x * x + y * y);
		}

		float SqrMagnitude() const 
		{
			return x * x + y * y;
		}

		Vector operator + (const Vector & a) const
		{
			Vector result;
			result.x = x + a.x;
			result.y = y + a.y;
			return result;
		}

		void operator += (const Vector & a) 
		{
			this->x += a.x;
			this->y += a.y;
		}
		Vector operator - (const Vector & a) const
		{
			Vector result;
			result.x = x - a.x;
			result.y = y - a.y;
			return result;
		}
		void operator -= (const Vector & a)
		{
			this->x -= a.x;
			this->y -= a.y;
		}
		T operator * (const Vector & a) const
		{
			T result = x * a.x + y * a.y;
			return result;
		}

		Vector operator * (T a) const
		{
			Vector result;
			result.x = this->x * a;
			result.y = this->y * a;
			return result;
		}

		Vector operator / (T a) const
		{
			Vector result;
			T b = 1 / a;
			result.x = this->x * b;
			result.y = this->y * b;
			return result;
		}

		bool operator == (const Vector & a) const
		{
			return this->x == a.x && this->y == a.y;
		}

		float GetEulerAngle() const
		{
			return std::atan2f(this->y, this->x);
		}

		void Set(T x,T y) 
		{
			this->x = x;
			this->y = y;
		}

		void Set(const Vector& value) {
			this->x = value.x;
			this->y = value.y;
		}

		static T Dot(const Vector& a,const Vector& b) 
		{
			T result = static_cast<T>(a.x * b.x + a.y * b.y);
			return result;
		}

	};
}

namespace noa
{
	class NOA_API Math
	{
	public:
		static const float pi;
		static const float halfPi;
		static const float doublePi;

		static float LinearLerp(float currentValue,float targetValue,float speed);

		static noa::Vector<float> FindArithmeticMean(
			const std::vector<noa::Vector<float>>& vertices
		);

		static bool IntersectPolygons(
			const std::vector<noa::Vector<float>>& verticesA
			, const std::vector<noa::Vector<float>>& verticesB
			, noa::Vector<float>* normal
			, float* depth
		);

		static void ProjectVertices(
			const std::vector<noa::Vector<float>>& vertices
			,noa::Vector<float>& axis
			,float* min
			,float* max
		);

	};
}




#endif

