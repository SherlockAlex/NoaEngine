#include "Item.h"

Item::Item(Sprite* sprite) :GameObject(sprite), Rigidbody(this) 
{
	useGravity = false;
	collision.isTrigger = true;
	tag = "item";
}

void Item::OnTrigger(Collision other)
{
	Rigidbody* rigid = other.other;
	if (rigid->tag == "Player")
	{
		RemoveRigidbody();
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
	pickEvent.Invoke();
}