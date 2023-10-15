#include "Enimy.h"

Enimy::Enimy(Scene* scene, Sprite* sprite, Transform* player, LiveEntity* enimy) :
	Actor(scene)
{

	tag = "Enimy";

	this->player = player;
	rigid->useGravity = false;
	this->enimy = enimy;

	sprite->scale = { 32,32 };
	this->spriteRenderer->SetSprite(sprite);
	this->sprite = sprite;

}

Enimy::~Enimy()
{
	if (sprite)
	{
		delete sprite;
		sprite = nullptr;
	}
}


void Enimy::Update()
{
	if (currentAnimation!=nullptr) 
	{
		this->spriteRenderer->UpdateSprite(currentAnimation->GetCurrentFrameImage());
	}
	
}

void Enimy::OnDestroy()
{

}
