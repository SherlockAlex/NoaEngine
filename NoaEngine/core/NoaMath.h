#ifndef NOAENGINE_NOAMATH_H
#define NOAENGINE_NOAMATH_H

#include <math.h>
#include <string>
#include <unordered_map>

namespace noa {
// Math Constance
#define PI 3.1415926
#define HALFPI 1.57079633
#define MUTIPI 6.28318531
#define Min(a,b) ((a>b)?b:a)
#define Max(a,b) ((a>b)?a:b)

//定义2维矢量
	template <class T>
	struct Vector {
		T x;
		T y;


		Vector()
		{

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
			const float invLength =1.0 / sqrtf(this->x * this->x + this->y * this->y);
			Vector result;
			result.x = (float)this->x * invLength;
			result.y = (float)this->y * invLength;
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
			//向量点乘
			T result = x * a.x + y * a.y;
			return result;
		}

		Vector operator * (T a) const
		{
			//向量乘常数
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

	};
}

///返回的是T的绝对值，注意T只能是数值类型，否则报错
template<class T>
inline T NoaAbs(T value) 
{
	return (value > 0) ? value : (-value);
}

inline int NoaFastAbs(const int x)
{
	// 检查输入值的符号位
	// 获取符号位，正数为0，负数为-1

	return ((x ^ (x >> (sizeof(int) * 8 - 1))) - (x >> (sizeof(int) * 8 - 1)));
}

#define NOAABS(x) ((x ^ (x >> (sizeof(int) * 8 - 1))) - (x >> (sizeof(int) * 8 - 1)))

template <typename KeyType, typename ValueType>
inline bool ContainKey(const std::unordered_map<KeyType, ValueType>& myMap, const KeyType& keyToCheck) 
{
	return myMap.count(keyToCheck) > 0;
}

#endif

