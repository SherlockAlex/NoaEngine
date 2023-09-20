#include "Bullet.h"
#include "Player.h"

Bullet::Bullet(Scene * scene,Sprite* sprite) :GameObject(scene,sprite, {32,32})
{
	rigid->collision.sacle = { -0.3,-0.3 };
	rigid->collision.radius = 0.2;
	rigid->collision.isTrigger = true;
	rigid->useGravity = false;
	rigid->tag = "Bullet";
	
}

Bullet* Bullet::Create(Scene* scene, Sprite* sprite)
{
	return new Bullet(scene, sprite);
}

Bullet::~Bullet()
{
	GameObject::~GameObject();
	//Rigidbody::~Rigidbody();
}

void Bullet::Update()
{
	rigid->velocity = dir.Normalize() * speed;
}

void Bullet::OnHitTile()
{
	Destroy();
	rigid->Destroy();
}

void Bullet::OnTrigger(Collision other)
{
	Rigidbody* rigid = other.other;

	Player* player = rigid->GetActorAs<Player*>();
	if (player!=nullptr&&rigid->tag == "Player")
	{
		player->TakeDamage(3);
		this->rigid->Destroy();
		Destroy();
	}

}
