#ifndef NOAENGINE_NOAMATH_H
#define NOAENGINE_NOAMATH_H

#include <math.h>
#include <string>
#include <unordered_map>

namespace noa {
	// Math Constance
#define PI 3.14159

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

		Vector Normalize() 
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

		Vector operator + (Vector a)
		{
			Vector result;
			result.x = x + a.x;
			result.y = y + a.y;
			return result;
		}

		void operator += (Vector a) 
		{
			this->x += a.x;
			this->y += a.y;
		}
		Vector operator - (Vector a)
		{
			Vector result;
			result.x = x - a.x;
			result.y = y - a.y;
			return result;
		}
		void operator -= (Vector a)
		{
			this->x -= a.x;
			this->y -= a.y;
		}
		T operator * (Vector a)
		{
			//向量点乘
			T result = x * a.x + y * a.y;
			return result;
		}

		Vector operator * (T a)
		{
			//向量乘常数
			Vector result;
			result.x = this->x * a;
			result.y = this->y * a;
			return result;
		}

		Vector operator / (T a)
		{
			Vector result;
			T b = 1 / a;
			result.x = this->x * b;
			result.y = this->y * b;
			return result;
		}

		bool operator == (Vector a) {
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

template <typename KeyType, typename ValueType>
inline bool ContainKey(const std::unordered_map<KeyType, ValueType>& myMap, const KeyType& keyToCheck) 
{
	return myMap.count(keyToCheck) > 0;
}

#endif

