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

		void SetSprite(Sprite * sprite);
		void Update() override;

	};
}

#endif // !NOAENGINE_SPRITERENDERER_H



