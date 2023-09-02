#ifndef NOAENGINE_GAMEOBJECT
#define NOAENGINE_GAMEOBJECT

#include "Behaviour.h"
#include "Sprite.h"

namespace noa {
	class GameObject :public Behaviour
	{
		//游戏内的物品
		//制动绘制游戏物品到屏幕上
	public:
		Sprite * sprite = nullptr;
		bool isTrigger = false;
	public:
		GameObject(Sprite * sprite);

		GameObject(Sprite * sprite, Vector<float> startPosition);

		~GameObject();

		//用于检测碰撞并传递消息
		virtual void OnTriggerEnter(vector<void*> other) {
			//用于传递碰撞检测的消息
		}

		virtual void RenderGameObject() {
			//实现绘制物品信息
			sprite->DrawSprite(transform.position.x, transform.position.y);
		}

	};

	extern void Destroy(GameObject* gameObject);
}



#endif // !NOAENGINE_GAMEOBJECT

