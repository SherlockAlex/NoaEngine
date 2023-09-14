#include "Item.h"

Item::Item(Sprite* sprite) :GameObject(sprite), Rigidbody(&transform) 
{
	useGravity = false;
	collision.isTrigger = true;
	tag = "item";
}

void Item::OnTrigger(void* other)
{
	if (other == nullptr)
	{
		return;
	}
	Rigidbody* rigid = (Rigidbody*)other;
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