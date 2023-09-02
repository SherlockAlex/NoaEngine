#ifndef NOAENGINE_BEHAVIOUR_H
#define NOAENGINE_BEHAVIOUR_H

#include <vector>
//#include "NoaMath.h"
#include "Transform.h"

using namespace std;

namespace noa {
	//��Ϸ�ű���

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

	//������Ϸ�ű�
	extern void Destroy(Behaviour* behaviour);
}



#endif
