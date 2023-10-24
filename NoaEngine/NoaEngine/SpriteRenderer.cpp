#include "Actor.h"
#include "Sprite.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Debug.h"

namespace noa {
	std::vector<noa::SpriteRendererInstance> spriteRendererInstances;
}


noa::SpriteRenderer::SpriteRenderer(Actor* actor):ActorComponent(actor)
{

}

noa::SpriteRenderer::~SpriteRenderer()
{
	if (spriteGPU!=nullptr) 
	{
		delete spriteGPU;
		spriteGPU = nullptr;
	}
}

noa::SpriteRenderer* noa::SpriteRenderer::Create(Actor* actor)
{
	return new SpriteRenderer(actor);
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
		spriteGPU = new SpriteGPU(sprite);
		return;
	}

	spriteGPU->Update(sprite);
	
}

void noa::SpriteRenderer::SetAnimation(Animation* animation)
{
	if (animation == nullptr) 
	{
		return;
	}
	animationComponent = animation;

}

void noa::SpriteRenderer::Update()
{
	if (sprite == nullptr) 
	{
		noa::Debug::Warring("The sprite of SpriteRenderer is null");
		return;
	}

	if (animationComponent) 
	{
		this->sprite->UpdateImage(animationComponent->GetCurrentFrameImage());
	}

	SpriteRendererInstance instance;
	instance.actor = this->GetActor();
	instance.sprite = this->sprite;
	instance.spriteGPU = this->spriteGPU;

	spriteRendererInstances.push_back(instance);

}
