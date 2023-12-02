#ifndef NOAENGINE_SPRITERENDERER_H
#define NOAENGINE_SPRITERENDERER_H

#include "ActorComponent.h"
#include "Sprite.h"
#include "Graphic.h"
#include "Core.h"

namespace noa {

	typedef struct NOA_API SpriteRendererInstance {
		Actor* actor = nullptr;
		Vector<float> offset;
		Vector<int> spriteSize = {0,0};
		SpriteGPU* spriteGPU = nullptr;
		uint32_t tint = noa::WHITE;
		float distanceToPlayer = 0;
		Vector<float> vecToPlayer = { 0,0 };
		Vector<bool> isFlip = {false,false};
		Vector<float> scale = { 1.0f,1.0f };

	}SpriteRendererInstance;

	class NOA_API UITransform;
	class NOA_API SpriteRenderer:public ActorComponent
	{
	private:
		ACTOR_COMPONENT(SpriteRenderer)
	private:
		InstanceLayer layer = InstanceLayer::GAME_LAYER;
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
		SpriteRenderer& SetOffset(float x, float y);
		SpriteRenderer& SetLayer(InstanceLayer layer);

		SpriteRenderer* Apply();

		void Update() override;
		void Render() override;

	public:
		Vector<int> spriteOriginSize = {0,0};
		uint32_t tint = noa::WHITE;
		Vector<bool> isFlip = {false,false};
		Vector<float> scale = {1.0f,1.0f};
		Vector<float> offset = { 0.0f,0.0f };

	};

	extern std::vector<SpriteRendererInstance> spriteRendererInstances;

}

#endif // !NOAENGINE_SPRITERENDERER_H



