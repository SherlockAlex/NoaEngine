#ifndef NOAENGINE_TRANSFORM_H
#define NOAENGINE_TRANSFORM_H
#include "NoaMath.h"

namespace noa {
	class Transform final
	{
	public:
		Vector<float> position = {0,0};
		float eulerAngle = 0.0;
		float posZ = 0.0;
	};

	class UITransform final {
	public:
		Vector<int> position = { 0,0 };
		Vector<int> scale = {150,150};
	};
}

#endif // !NOAENGINE_TRANSFORM_H
