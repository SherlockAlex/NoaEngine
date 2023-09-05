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
	private:
		bool isActive = true;
	public:
		Transform transform;
		int hashCode = 0;

	public:
		Behaviour();
		~Behaviour();
	public:
		virtual void OnEnable() {}
		virtual void Start() {}
		virtual void Update() {}
		virtual void OnDisable() {}
		virtual void Destroy();
		void SetActive(bool value);
		bool GetActive();
	};

	//销毁游戏脚本
	//extern void Destroy(Behaviour* behaviour);
}



#endif
