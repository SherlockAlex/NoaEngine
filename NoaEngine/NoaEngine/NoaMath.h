#ifndef NOAENGINE_NOAMATH_H
#define NOAENGINE_NOAMATH_H

#include <math.h>
#include <string>
#include <unordered_map>
#include <map>

namespace noa {
// Math Constance
#define PI 3.1415926f
#define HALFPI 1.57079633f
#define MUTIPI 6.28318531f
#define Min(a,b) ((a>b)?b:a)
#define Max(a,b) ((a>b)?a:b)

//∂®“Â2Œ¨ ∏¡ø
	template <class T>
	struct Vector {
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
			const float invLength =1.0f / sqrtf(this->x * this->x + this->y * this->y);
			Vector result;
			result.x = static_cast<T>((float)this->x * invLength);
			result.y = static_cast<T>((float)this->y * invLength);
			return result;

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

	};
}

#define NOAABS(x) ((x ^ (x >> (sizeof(int) * 8 - 1))) - (x >> (sizeof(int) * 8 - 1)))

template <typename KeyType, typename ValueType>
inline bool ContainKey(const std::unordered_map<KeyType, ValueType>& myMap, const KeyType& keyToCheck) 
{
	return myMap.count(keyToCheck) > 0;
}

template <typename KeyType, typename ValueType>
inline bool ContainKey(const std::map<KeyType, ValueType>& myMap, const KeyType& keyToCheck)
{
	return myMap.count(keyToCheck) > 0;
}

namespace noa
{
	class Math
	{
	public:
		static const float Pi;
		static const float halfPi;
		static const float doublePi;

		static float LinearLerp(float currentValue,float targetValue,float stepSpeed);

	};
}




#endif

