#include "Actor.h"
#include "Sprite.h"
#include "SpriteRenderer.h"
#include "Debug.h"
#include "Transform.h"


std::vector<noa::SpriteRendererInstance> noa::spriteRendererInstances;


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
	spriteGPU->UpdateImage(spriteFile);
}

noa::SpriteRenderer& noa::SpriteRenderer::SetSprite(
	Sprite* sprite)
{
	if (sprite == nullptr) 
	{
		return *this;
	}
	this->spriteOriginSize.x = sprite->w;
	this->spriteOriginSize.y = sprite->h;
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

noa::SpriteRenderer& noa::SpriteRenderer::SetOffset(float x,float y) 
{
	this->offset.x = x;
	this->offset.y = y;
	return *this;
}

noa::SpriteRenderer& noa::SpriteRenderer::SetLayer(
	noa::InstanceLayer layer) 
{
	this->layer = layer;
	return *this;
}

noa::SpriteRenderer* noa::SpriteRenderer::Apply()
{
	return this;
}

void noa::SpriteRenderer::Update() {

}

void noa::SpriteRenderer::Render()
{

	spriteGPU->SetLayer(this->layer);
	SpriteRendererInstance instance;
	instance.actor = this->GetActor();
	instance.offset = this->offset;
	instance.tint = this->tint;
	instance.spriteSize = this->spriteOriginSize;
	instance.spriteGPU = this->spriteGPU.get();
	instance.scale = this->scale;
	instance.isFlip = this->isFlip;

	spriteRendererInstances.push_back(instance);

}
