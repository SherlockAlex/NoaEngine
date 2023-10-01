#ifndef NOAENGINE_SPRITERENDERER_H
#define NOAENGINE_SPRITERENDERER_H

#include "ActorComponent.h"
#include "Sprite.h"

namespace noa {

	typedef struct SpriteRendererInstance {
		Actor* actor = nullptr;
		Sprite* sprite = nullptr;
		SpriteGPU* spriteGPU = nullptr;
		float distanceToPlayer = 0;
		Vector<float> vecToPlayer = { 0,0 };

		Vector<bool> isFlip = {false,false};

	}SpriteRendererInstance;

	/// <summary>
	/// ������������Զ����ƾ��鵽��Ļ��
	/// </summary>
	class SpriteRenderer:public ActorComponent
	{
	private:
		Sprite* sprite = nullptr;
		SpriteGPU* spriteGPU = nullptr;
	private:
		SpriteRenderer(Actor* actor);
		~SpriteRenderer();

	public:

		static SpriteRenderer* Create(Actor * actor);
		void Delete() override;

		void UpdateSprite(const SpriteFile & spriteFile);
		void SetSprite(Sprite * sprite);

		void Update() override;

	};

	/*
	* spriteRendererInstances����Scene.Update()ִ�����Ժ���������գ�Ȼ����ActorUpdateִ��֮ǰ�����һ֡���ص�Sprite
	*/

	extern std::vector<SpriteRendererInstance> spriteRendererInstances;

}

#endif // !NOAENGINE_SPRITERENDERER_H



