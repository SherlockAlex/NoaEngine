#pragma once
#include "Behaviour.h"
#include "Sprite.h"

class GameObject :public Behaviour
{
	//游戏内的物品
public:
	Sprite sprite;
	bool isTrigger = false;
public:
	GameObject(Sprite sprite);

	GameObject(Sprite sprite, Vector startPosition);

	~GameObject();

	void Destroy() override;

	//用于检测碰撞并传递消息
	virtual void OnTriggerEnter(vector<void*> other) {
		
	}

};

extern vector<GameObject*> gameObjects;

