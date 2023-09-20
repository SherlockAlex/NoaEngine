#ifndef NOAENGINE_GAMEOBJECT
#define NOAENGINE_GAMEOBJECT

#include "Actor.h"
#include "Sprite.h"
#include "NoaMath.h"

namespace noa {
	class GameObject :public Actor
	{
		//��Ϸ�ڵ���Ʒ
		//�ƶ�������Ϸ��Ʒ����Ļ��
	public:
		//�ж��ܷ����߻���
		bool isRaycasted = false;
		Sprite * sprite = nullptr;
	protected:
		GameObject(Scene * activeScene,Sprite * sprite);

		GameObject(Scene* activeScene, Sprite * sprite, Vector<float> startPosition);

	public:
		static GameObject* Create(Scene* activeScene, Sprite* sprite);
		static GameObject* Create(Scene* activeScene, Sprite* sprite, Vector<float> startPosition);

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

