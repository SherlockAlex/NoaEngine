#ifndef NOAENGINE_SPRITERENDERER_H
#define NOAENGINE_SPRITERENDERER_H

#include "ActorComponent.h"

namespace noa {

	typedef struct SpriteRendererInstance {
		Actor* actor = nullptr;
		Sprite* sprite = nullptr;
		SpriteGPU* spriteGPU = nullptr;
		float distanceToPlayer = 0;
		Vector<float> vecToPlayer = { 0,0 };
		Vector<bool> isFlip = {false,false};

	}SpriteRendererInstance;

	class Animation;
	class SpriteRenderer:public ActorComponent
	{
	private:
		Sprite* sprite = nullptr;
		SpriteGPU* spriteGPU = nullptr;
		Animation* animationComponent = nullptr;
	private:
		SpriteRenderer(Actor* actor);
		~SpriteRenderer();

	public:
		static SpriteRenderer* Create(Actor * actor);

		void UpdateSprite(const SpriteFile & spriteFile);
		void SetSprite(Sprite * sprite);
		void SetAnimation(Animation* animation);

		void Update() override;

	};

	extern std::vector<SpriteRendererInstance> spriteRendererInstances;

}

#endif // !NOAENGINE_SPRITERENDERER_H



