#ifndef NOAENGINE_SCREEN_H
#define NOAENGINE_SCREEN_H

#include "NoaMath.h"
#include "Core.h"

namespace noa {
	class NOA_API Screen {
	private:
		friend class NoaEngine;
		friend class Renderer;
	public:
		static int width;
		static int height;
	private:
		static uint32_t * pixelBuffer;
	};
}



#endif // !NOAENGINE_SCREEN_H


