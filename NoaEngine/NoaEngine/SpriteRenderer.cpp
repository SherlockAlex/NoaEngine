#include "Actor.h"
#include "Sprite.h"
#include "SpriteRenderer.h"
#include "Debug.h"
#include "Transform.h"


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
	return NObject<SpriteRenderer>::Create(actor);
}

void noa::SpriteRenderer::UpdateSprite(const SpriteFile& spriteFile)
{
	if (sprite == nullptr)
	{
		return;
	}
	sprite->UpdateImage(spriteFile);
}

noa::SpriteRenderer& noa::SpriteRenderer::SetSprite(Sprite* sprite)
{
	if (sprite == nullptr) 
	{
		return *this;
	}
	this->sprite = sprite;

	if (spriteGPU == nullptr) 
	{
		spriteGPU = SpriteGPU::Create(sprite);
		return *this;
	}

	spriteGPU->Update(sprite);
	
	return *this;

}

noa::SpriteRenderer& noa::SpriteRenderer::SetFlipX(bool value) 
{
	this->isFlip.x = value;
	return *this;
}

noa::SpriteRenderer& noa::SpriteRenderer::SetFlipY(bool value) 
{
	this->isFlip.y = value;
	return *this;
}

noa::SpriteRenderer& noa::SpriteRenderer::SetScale(float x, float y)
{
	this->scale.x = x;
	this->scale.y = y;
	return *this;
}

noa::SpriteRenderer* noa::SpriteRenderer::Apply()
{
	return this;
}

void noa::SpriteRenderer::Update()
{

	

	if (!sprite) 
	{
		noa::Debug::Warring("The sprite of SpriteRenderer is null");
		return;
	}
	
	

	SpriteRendererInstance instance;
	instance.actor = this->GetActor();
	instance.sprite = this->sprite;
	instance.spriteGPU = this->spriteGPU.get();
	instance.scale = this->scale;
	instance.isFlip = this->isFlip;

	spriteRendererInstances.push_back(instance);

}
