#ifndef NOAENGINE_TRANSFORM_H
#define NOAENGINE_TRANSFORM_H
#include "NoaMath.h"

namespace noa {
	class Transform final
	{
	public:
		Vector<float> position = {0.0f,0.0f};		//绝对坐标系
		Vector<float> localPosition = {0.0f,0.0f};	//相对父的绝对坐标系
		float eulerAngle = 0.0f;
		float localEulerAngle = 0.0f;
		float posZ = 0.0;
	};

	class UITransform final {
	public:
		Vector<int> position = { 0,0 };
		Vector<int> size = {150,150};
	};
}

#endif // !NOAENGINE_TRANSFORM_H
