#ifndef NOAENGINE_GAMEOBJECT
#define NOAENGINE_GAMEOBJECT

#include "Actor.h"
#include "Sprite.h"
#include "NoaMath.h"

namespace noa {

	/// <summary>
	/// ���ӻ���࣬������Scene����Ϸ��Ʒ�����Ա�Cameraֱ����Ⱦ
	/// </summary>
	class GameObject :public Actor
	{
		//��Ϸ�ڵ���Ʒ
		//�ƶ�������Ϸ��Ʒ����Ļ��
	public:
		//�ж��ܷ����߻���
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

