#include "Item.h"

Item::Item(Scene* scene, Sprite* sprite) :Actor(scene)
{
	rigid->useGravity = false;
	collider->isTrigger = true;
	tag = "item";

	sprite->scale = { 32,32 };
	spriteRenderer->SetSprite(sprite);

}

Item::~Item()
{
	
}

Item* Item::Create(Scene* scene, Sprite* sprite)
{
	return new Item(scene, sprite);
}

void Item::OnTrigger(const Collision & other)
{
	if (other.CompareTag("Player"))
	{
		//这个地方有问题
		pickEvent.Invoke();
		Destroy();
		
	}
}

void Item::Update()
{
	this->transform.posZ *= 0.85;
}

void Item::OnDisable()
{
	
}