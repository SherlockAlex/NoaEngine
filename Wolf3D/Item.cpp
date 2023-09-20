#include "Item.h"

Item::Item(Scene* scene, Sprite* sprite) :GameObject(scene,sprite)
{
	rigid->useGravity = false;
	rigid->collision.isTrigger = true;
	rigid->tag = "item";
}

void Item::OnTrigger(Collision other)
{
	Rigidbody* rigid = other.other;
	if (rigid->tag == "Player")
	{
		pickEvent.Invoke();
		this->rigid->Destroy();
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