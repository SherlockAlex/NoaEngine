#ifndef NOAENGINE_GAMEOBJECT
#define NOAENGINE_GAMEOBJECT

#include "Behaviour.h"
#include "Sprite.h"

namespace noa {
	class GameObject :public Behaviour
	{
		//��Ϸ�ڵ���Ʒ
		//�ƶ�������Ϸ��Ʒ����Ļ��
	public:
		Sprite * sprite = nullptr;
		bool isTrigger = false;
	public:
		GameObject(Sprite * sprite);

		GameObject(Sprite * sprite, Vector<float> startPosition);

		~GameObject();

		//���ڼ����ײ��������Ϣ
		virtual void OnTriggerEnter(vector<void*> other) {
			//���ڴ�����ײ������Ϣ
		}

		virtual void RenderGameObject() {
			//ʵ�ֻ�����Ʒ��Ϣ
			sprite->DrawSprite(transform.position.x, transform.position.y);
		}

	};

	extern void Destroy(GameObject* gameObject);
}



#endif // !NOAENGINE_GAMEOBJECT

