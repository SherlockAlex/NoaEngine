#include "Actor.h"
#include "Sprite.h"
#include "SpriteRenderer.h"
#include "Debug.h"
#include "Transform.h"

/**
* SpriteRenderer组件
* 负责将Sprite绘制到屏幕上
* 但是其绘制必须依赖相机
* 一个使用方式就是使用UI空间Image
* 而SpriteRenderer是专门为2D游戏开发准备的
*/

namespace noa {
	std::vector<noa::SpriteRendererInstance> spriteRendererInstances;
}


noa::SpriteRenderer::SpriteRenderer(Actor* actor):ActorComponent(actor)
{

}

noa::SpriteRenderer::~SpriteRenderer()
{
	
}

noa::SpriteRenderer* noa::SpriteRenderer::Create(Actor* actor)
{
	return NObject<SpriteRenderer>::Create<Actor*>(actor);
}

void noa::SpriteRenderer::UpdateSprite(const SpriteFile& spriteFile)
{
	if (sprite == nullptr)
	{
		return;
	}
	sprite->UpdateImage(spriteFile);
}

void noa::SpriteRenderer::SetSprite(Sprite* sprite)
{
	if (sprite == nullptr) 
	{
		return;
	}
	this->sprite = sprite;

	if (spriteGPU == nullptr) 
	{
		spriteGPU = SpriteGPU::Create(sprite);
		return;
	}

	spriteGPU->Update(sprite);
	
}

void noa::SpriteRenderer::Update()
{
	if (sprite == nullptr) 
	{
		noa::Debug::Warring("The sprite of SpriteRenderer is null");
		return;
	}

	SpriteRendererInstance instance;
	instance.actor = this->GetActor();
	instance.sprite = this->sprite;
	instance.spriteGPU = this->spriteGPU.get();

	spriteRendererInstances.push_back(instance);

}
