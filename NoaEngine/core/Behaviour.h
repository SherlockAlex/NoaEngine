#ifndef NOAENGINE_BEHAVIOUR_H
#define NOAENGINE_BEHAVIOUR_H

#include <vector>
#include "NoaMath.h"

using namespace std;

namespace noa {
	//游戏脚本类
	class Behaviour {
	public:
		//表示位置信息
		Vector<float> position;
		float angle = 0.0;
		bool isActive = true;

	public:
		Behaviour();
		~Behaviour();
	public:
		virtual void Start() = 0;
		virtual void Update() = 0;
	};

	//销毁游戏脚本
	extern void Destroy(Behaviour* behaviour);
}



#endif
