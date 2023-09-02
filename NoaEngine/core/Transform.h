#ifndef NOAENGINE_TRANSFORM_H
#define NOAENGINE_TRANSFORM_H
#include "NoaMath.h"

namespace noa {
	class Transform
	{
	public:
		Vector<float> position;
		//the euler angle in the world with behaviour
		float eulerAngle = 0.0;
	};
}

#endif // !NOAENGINE_TRANSFORM_H
