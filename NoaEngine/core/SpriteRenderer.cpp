#include "SpriteRenderer.h"

noa::SpriteRenderer::SpriteRenderer(Actor* actor):ActorComponent(actor)
{

}

noa::SpriteRenderer::~SpriteRenderer()
{
}

noa::SpriteRenderer* noa::SpriteRenderer::Create(Actor* actor)
{
	return new SpriteRenderer(actor);
}

void noa::SpriteRenderer::Delete()
{
	delete this;
}

void noa::SpriteRenderer::SetSprite(Sprite* sprite)
{
}

void noa::SpriteRenderer::Update()
{
	SpriteRendererInstance instance;
	instance.actor = this->GetActorAs<Actor*>();
	instance.sprite = this->sprite;
	instance.spriteGPU = this->spriteGPU;
}
