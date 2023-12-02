#ifndef NOAENGINE_FONT_H
#define NOAENGINE_FONT_H

#include <vector>
#include <memory>

#include "Core.h"
#include "NoaMath.h"

namespace noa {
	//字体

	struct NOA_API SpriteFile;

	class NOA_API SpriteGPU;

	typedef struct NOA_API Font
	{
		int w = 0;
		int h = 0;
		std::shared_ptr<SpriteGPU> spriteGPU = nullptr;

		noa::Vector<int> bearing;	//从基准线到字形左部/顶部的偏移值
		uint32_t advance = 0;			//原点距下一个字形原点的距离

		Font();

		Font(const SpriteFile& spriteFile);

	}Font;
}

#endif // !NOAENGINE_FONT_H

