#ifndef NOAENGINE_TRANSFORM_H
#define NOAENGINE_TRANSFORM_H
#include "NoaMath.h"
#include "Core.h"

namespace noa {
	class NOA_API Transform final
	{
	public:
		Vector<float> position = {0.0f,0.0f};
		float eulerAngle = 0.0f;
		float posZ = 0.0;
	};

	class NOA_API UITransform final {
	public:
		Vector<int> position = { 0,0 };
		Vector<int> size = {150,150};
	};
}

#endif // !NOAENGINE_TRANSFORM_H
