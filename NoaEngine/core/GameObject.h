#ifndef NOAENGINE_GAMEOBJECT
#define NOAENGINE_GAMEOBJECT

#include "Behaviour.h"
#include "Sprite.h"
#include "NoaMath.h"

namespace noa {
	class GameObject :public Behaviour
	{
		//��Ϸ�ڵ���Ʒ
		//�ƶ�������Ϸ��Ʒ����Ļ��
	public:
		
		Sprite * sprite = nullptr;
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

