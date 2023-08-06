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

	virtual void OnTriggerEnter(vector<void*> other) {

	}

};

extern vector<GameObject*> gameObjects;

