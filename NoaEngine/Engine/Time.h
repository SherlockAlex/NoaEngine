#ifndef NOAENGINE_TIME_H
#define NOAENGINE_TIME_H
#include "Core.h"

namespace noa {
	class NOA_API Time
	{
	public:
		static float realDeltaTime;
		static float deltaTime;
		static float timeScale;
		static float time;
		static int fps;
	};
}



#endif
