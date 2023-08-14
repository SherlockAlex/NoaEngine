#ifndef NOAENGINE_GAMEOBJECT
#define NOAENGINE_GAMEOBJECT

#include "Behaviour.h"
#include "Sprite.h"

class GameObject :public Behaviour
{
	//��Ϸ�ڵ���Ʒ
	//�ƶ�������Ϸ��Ʒ����Ļ��
public:
	Sprite sprite;
	bool isTrigger = false;
public:
	GameObject(Sprite sprite);

	GameObject(Sprite sprite, Vector<float> startPosition);

	~GameObject();

	void Destroy() override;

	//���ڼ����ײ��������Ϣ
	virtual void OnTriggerEnter(vector<void*> other) {

	}

};

extern vector<GameObject*> gameObjects;

#endif // !NOAENGINE_GAMEOBJECT

