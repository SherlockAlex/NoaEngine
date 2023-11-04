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
		Vector<float> scale = { 1.0f,1.0f };

	}SpriteRendererInstance;

	class UITransform;
	class SpriteRenderer:public ActorComponent
	{
	private:
		NOBJECT(SpriteRenderer)
	private:
		
		Sprite* sprite = nullptr;
		std::shared_ptr<SpriteGPU> spriteGPU = nullptr;

	private:
		SpriteRenderer(Actor* actor);
		~SpriteRenderer();

	public:
		static SpriteRenderer* Create(Actor * actor);

		void UpdateSprite(const SpriteFile & spriteFile);

		SpriteRenderer& SetSprite(Sprite * sprite);
		SpriteRenderer& SetScale(float x,float y);

		SpriteRenderer* Apply();

		void Update() override;

	public:
		Vector<bool> isFlip = {false,false};
		Vector<float> scale = {1.0f,1.0f};
	};

	extern std::vector<SpriteRendererInstance> spriteRendererInstances;

}

#endif // !NOAENGINE_SPRITERENDERER_H



