#pragma once
#include "Behaviour.h"
#include "Sprite.h"

class GameObject :public Behaviour
{
	//��Ϸ�ڵ���Ʒ
public:
	Sprite sprite;
	bool isTrigger = false;
public:
	GameObject(Sprite sprite);

	GameObject(Sprite sprite, Vector startPosition);

	~GameObject();

	void Destroy() override;

	//���ڼ����ײ��������Ϣ
	virtual void OnTriggerEnter(vector<void*> other) {
		
	}

};

extern vector<GameObject*> gameObjects;

