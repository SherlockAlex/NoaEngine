#ifndef NOAENGINE_BEHAVIOUR_H
#define NOAENGINE_BEHAVIOUR_H

#include <vector>
#include "NoaMath.h"

using namespace std;

namespace noa {
	//��Ϸ�ű���
	class Behaviour {
	public:
		//��ʾλ����Ϣ
		Vector<float> position;

	public:
		Behaviour();
		~Behaviour();
	public:
		virtual void Start() = 0;
		virtual void Update() = 0;
	};

	//������Ϸ�ű�
	extern void Destroy(Behaviour* behaviour);
}



#endif
