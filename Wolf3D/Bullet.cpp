#include "Bullet.h"
#include "Player.h"
#include "WolfResource.h"

Bullet::Bullet(Scene * scene,Sprite* sprite) :Actor(scene)
{
	animation->SetFrame(&wolfResource->bulletDestroyFrame);

	rigid->collision.sacle = { -0.3,-0.3 };
	collider->radius = 0.2;
	collider->isTrigger = true;
	rigid->useGravity = false;
	tag = "Bullet";

	sprite->scale = { 32,32 };
	spriteRenderer->SetSprite(sprite);
	
	animation->SetFrameEvent(3,
		[this]() 
		{
			Destroy();
		}
	);

}

Bullet* Bullet::Create(Scene* scene, Sprite* sprite)
{
	return new Bullet(scene, sprite);
}

Bullet::~Bullet()
{
	
}

void Bullet::Update()
{
	rigid->velocity = dir.Normalize() * speed;
	this->spriteRenderer->UpdateSprite(animation->GetCurrentFrameImage());
}

void Bullet::OnHitTile()
{
	rigid->SetActive(false);
	animation->Play();
}

void Bullet::OnTrigger(const Collision & other)
{
	Rigidbody* rigid = other.other;

	Player* player = nullptr;
	if (rigid->TryGetActorAs<Player>(player)&&rigid->CompareTag("Player"))
	{
		player->TakeDamage(3);
		Destroy();
	}

}
