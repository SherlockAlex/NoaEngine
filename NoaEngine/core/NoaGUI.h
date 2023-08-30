#ifndef NOAGUI_H
#define NOAGUI_H

#include "Sprite.h"
#include "NoaMath.h"
#include "NoaAction.h"

namespace noa {
	class NoaButton 
	{
	public:
		//��Ļλ��
		Vector<int> position;
		//��ȸ߶�
		Vector<int> scale;
		uint32_t normalColor;
		uint32_t selectColor;
		NoaEvent<void()> clickEvent;
	};
}

#endif // !NOAGUI_H


