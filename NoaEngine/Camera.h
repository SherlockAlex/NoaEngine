#ifndef NOAENGINE_CAMERA_H
#define NOAENGINE_CAMERA_H

#include "NoaMath.h"

namespace noa {
	//�������Ϸ�������������ʾ��Ϸ�Ļ�������
	//��Ҫ��������Ⱦ��Ϸ�ĳ���
	// Խ��ǰ�ľ��ȱ���Ⱦ
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


