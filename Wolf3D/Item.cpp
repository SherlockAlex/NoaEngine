#include "Item.h"

Item::Item(Scene* scene, Sprite* sprite) :GameObject(scene,sprite)
{
	rigid->useGravity = false;
	rigid->collision.isTrigger = true;
	rigid->tag = "item";
}

Item::~Item()
{
	Debug("Remove Item");
}

Item* Item::Create(Scene* scene, Sprite* sprite)
{
	return new Item(scene, sprite);
}

void Item::Delete()
{
	delete this;
}

void Item::OnTrigger(Collision other)
{
	Rigidbody* rigid = other.other;
	if (rigid!=nullptr&&rigid->tag == "Player")
	{
		//这个地方有问题
		pickEvent.Invoke();
		Destroy();
		Debug("Pick up the bullet");
	}
}

void Item::Update()
{
	this->transform.posZ *= 0.85;
}

void Item::OnDisable()
{
	
}