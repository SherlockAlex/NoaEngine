#ifndef NOAENGINE_CAMERA_H
#define NOAENGINE_CAMERA_H

#include "NoaMath.h"

namespace noa {
	//这个是游戏的相机，用来显示游戏的画面内容
	//主要是用来渲染游戏的场景
	// 越靠前的就先被渲染
	// skybox
	// levelMap
	// gameobject
	class Carema
	{
	public:
		Vector<float>* follow = nullptr;
		Vector<float> followSize;
		Vector<float> position;
	};
}


#endif // !NOAENGINE_CAMERA_H


