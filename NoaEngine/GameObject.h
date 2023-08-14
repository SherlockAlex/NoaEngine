#ifndef NOAENGINE_GAMEOBJECT
#define NOAENGINE_GAMEOBJECT

#include "Behaviour.h"
#include "Sprite.h"

class GameObject :public Behaviour
{
	//游戏内的物品
	//制动绘制游戏物品到屏幕上
public:
	Sprite sprite;
	bool isTrigger = false;
public:
	GameObject(Sprite sprite);

	GameObject(Sprite sprite, Vector<float> startPosition);

	~GameObject();

	void Destroy() override;

	//用于检测碰撞并传递消息
	virtual void OnTriggerEnter(vector<void*> other) {
		//用于传递碰撞检测的消息
	}

};

#endif // !NOAENGINE_GAMEOBJECT

