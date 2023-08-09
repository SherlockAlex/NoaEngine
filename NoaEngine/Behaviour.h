#ifndef NOAENGINE_BEHAVIOUR_H
#define NOAENGINE_BEHAVIOUR_H

#include <vector>
#include "NoaMath.h"

using namespace std;

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
	virtual void Destroy();
};

extern vector <Behaviour*> behaviours;

#endif
