#ifndef NOAENGINE_SPRITERENDERER_H
#define NOAENGINE_SPRITERENDERER_H

#include "ActorComponent.h"
#include "Sprite.h"
#include "Graphic.h"

namespace noa {

	typedef struct SpriteRendererInstance {
		Actor* actor = nullptr;
		Sprite* sprite = nullptr;
		SpriteGPU* spriteGPU = nullptr;
		uint32_t tint = noa::WHITE;
		float distanceToPlayer = 0;
		Vector<float> vecToPlayer = { 0,0 };
		Vector<bool> isFlip = {false,false};
		Vector<float> scale = { 1.0f,1.0f };

	}SpriteRendererInstance;

	class UITransform;
	class SpriteRenderer:public ActorComponent
	{
	private:
		ACTOR_COMPONENT(SpriteRenderer)
	private:
		InstanceLayer layer = InstanceLayer::GAME_LAYER;
		Sprite* sprite = nullptr;
		std::shared_ptr<SpriteGPU> spriteGPU = nullptr;

	private:
		SpriteRenderer(Actor* actor);
		~SpriteRenderer();

	public:
		static SpriteRenderer* Create(Actor * actor);

		void UpdateSprite(const SpriteFile & spriteFile);

		SpriteRenderer& SetSprite(Sprite * sprite);
		SpriteRenderer& SetFlipX(bool value);
		SpriteRenderer& SetFlipY(bool value);
		SpriteRenderer& SetScale(float x,float y);
		SpriteRenderer& SetLayer(InstanceLayer layer);

		SpriteRenderer* Apply();

		void Update() override;
		void Render() override;

	public:
		uint32_t tint = noa::WHITE;
		Vector<bool> isFlip = {false,false};
		Vector<float> scale = {1.0f,1.0f};
	};

	extern std::vector<SpriteRendererInstance> spriteRendererInstances;

}

#endif // !NOAENGINE_SPRITERENDERER_H



