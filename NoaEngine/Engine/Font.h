#ifndef NOAENGINE_FONT_H
#define NOAENGINE_FONT_H

#include <vector>
#include <memory>

#include "Core.h"
#include "NoaMath.h"

namespace noa {
	//����

	struct NOA_API SpriteFile;

	class NOA_API SpriteGPU;

	typedef struct NOA_API Font
	{
		int w = 0;
		int h = 0;
		std::shared_ptr<SpriteGPU> spriteGPU = nullptr;

		noa::Vector<int> bearing;	//�ӻ�׼�ߵ�������/������ƫ��ֵ
		uint32_t advance = 0;			//ԭ�����һ������ԭ��ľ���

		Font();

		Font(const SpriteFile& spriteFile);

	}Font;
}

#endif // !NOAENGINE_FONT_H

