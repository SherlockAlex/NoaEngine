#ifndef NOAENGINE_GAMEOBJECT
#define NOAENGINE_GAMEOBJECT

#include "Actor.h"
#include "Sprite.h"
#include "NoaMath.h"

namespace noa {
	class GameObject :public Actor
	{
		//游戏内的物品
		//制动绘制游戏物品到屏幕上
	public:
		//判断能否被射线击中
		bool isRaycasted = false;
		Sprite * sprite = nullptr;
	public:
		GameObject(Scene * activeScene,Sprite * sprite);

		GameObject(Scene* activeScene, Sprite * sprite, Vector<float> startPosition);

		~GameObject();

		void Destroy() override;

	};

	//extern void Destroy(GameObject* gameObject);

	typedef struct GameObjectBuffer {
		GameObject* object;
		float distanceToPlayer;
		Vector<float> vecToPlayer;
	}GameObjectBuffer;

}



#endif // !NOAENGINE_GAMEOBJECT

