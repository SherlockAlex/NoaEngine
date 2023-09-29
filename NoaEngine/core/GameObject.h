#ifndef NOAENGINE_GAMEOBJECT
#define NOAENGINE_GAMEOBJECT

#include "Actor.h"
#include "Sprite.h"
#include "NoaMath.h"

namespace noa {

	/// <summary>
	/// 可视化活动类，可以在Scene中游戏物品，可以被Camera直接渲染
	/// </summary>
	class GameObject :public Actor
	{
		//游戏内的物品
		//制动绘制游戏物品到屏幕上
	public:
		//判断能否被射线击中
		bool isRaycasted = false;
		Sprite * sprite = nullptr;
		SpriteGPU* spriteGPU = nullptr;
	protected:
		GameObject(Scene * activeScene,Sprite * sprite);

		GameObject(Scene* activeScene, Sprite * sprite, Vector<float> startPosition);

	public:
		static GameObject* Create(Scene* activeScene, Sprite* sprite);
		static GameObject* Create(Scene* activeScene, Sprite* sprite, Vector<float> startPosition);
		virtual void Delete() override;

		virtual ~GameObject();

		void Destroy() override;

	};

	//extern void Destroy(GameObject* gameObject);

	typedef struct GameObjectBuffer {
		GameObject* object = nullptr;
		float distanceToPlayer = 0;
		Vector<float> vecToPlayer = {0,0};
	}GameObjectBuffer;

}



#endif // !NOAENGINE_GAMEOBJECT

