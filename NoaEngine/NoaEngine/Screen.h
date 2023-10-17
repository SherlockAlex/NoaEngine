#ifndef NOAENGINE_SCREEN_H
#define NOAENGINE_SCREEN_H

#include "NoaMath.h"

namespace noa {
	class Screen {
	private:
		friend class NoaEngine;
		friend class Renderer;
	public:
		static int width;
		static int height;
		static Vector<int> hardwareScreenScale;
	private:
		static uint32_t * pixelBuffer;
	};
}



#endif // !NOAENGINE_SCREEN_H


