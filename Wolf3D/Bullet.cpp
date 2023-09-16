#include "Bullet.h"
#include "Player.h"

Bullet::Bullet(Sprite* sprite) :GameObject(sprite, {32,32}),Rigidbody(this)
{
	collision.sacle = { -0.3,-0.3 };
	collision.radius = 0.2;
	collision.isTrigger = true;
	this->useGravity = false;
	tag = "Bullet";
	
}

Bullet::~Bullet()
{
	GameObject::~GameObject();
	Rigidbody::~Rigidbody();
}

void Bullet::Update()
{
	velocity = dir.Normalize() * speed;
}

void Bullet::OnHitTile()
{
	Destroy();
	RemoveRigidbody();
}

void Bullet::OnTrigger(Collision other)
{
	Rigidbody* rigid = other.other;

	Player* player = rigid->GetActorAs<Player*>();
	if (player->tag == "Player")
	{
		player->TakeDamage(3);
		Destroy();
		RemoveRigidbody();
	}

}
