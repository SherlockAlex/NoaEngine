#ifndef NOAENGINE_BEHAVIOUR_H
#define NOAENGINE_BEHAVIOUR_H

#include <vector>
//#include "NoaMath.h"
#include "Transform.h"

using namespace std;

namespace noa {
	//游戏脚本类

	class Transform;

	class Behaviour {
	public:
		Transform transform;
		////the position in the world with behaviour
		//Vector<float> position;
		////the euler angle in the world with behaviour
		//float angle = 0.0;
		bool isActive = true;

	public:
		Behaviour();
		~Behaviour();
	public:
		virtual void Start() {};
		virtual void Update() {};
	};

	//销毁游戏脚本
	extern void Destroy(Behaviour* behaviour);
}



#endif
