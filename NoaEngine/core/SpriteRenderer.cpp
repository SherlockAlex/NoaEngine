#include "Actor.h"
#include "SpriteRenderer.h"
#include "NoaEngine.h"

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

void noa::SpriteRenderer::Delete()
{
	delete this;
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
		
		return;
	}
	
}

void noa::SpriteRenderer::Update()
{
	if (sprite == nullptr) 
	{
		noa::Debug::Warring("The sprite of SpriteRenderer is null");
		return;
	}

	SpriteRendererInstance instance;
	instance.actor = this->actor;
	instance.sprite = this->sprite;
	instance.spriteGPU = this->spriteGPU;

	spriteRendererInstances.push_back(instance);

}
