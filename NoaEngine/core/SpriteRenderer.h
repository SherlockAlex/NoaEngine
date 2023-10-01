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
	/// 用于让摄像机自动绘制精灵到屏幕上
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
	* spriteRendererInstances会在Scene.Update()执行完以后立即被清空，然后在ActorUpdate执行之前获得下一帧加载的Sprite
	*/

	extern std::vector<SpriteRendererInstance> spriteRendererInstances;

}

#endif // !NOAENGINE_SPRITERENDERER_H



