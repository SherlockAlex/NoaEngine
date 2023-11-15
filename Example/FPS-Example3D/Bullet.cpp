#include "Bullet.h"
#include "Player.h"
#include "WolfResource.h"

Bullet::Bullet(Scene * scene,Sprite* sprite) :Actor(scene)
{
	animation->SetClip(&wolfResource->bulletDestroyFrame);
	this->animation->SetSpeed(20);
	this->animation->SetLoop(false);

	tileCollider->SetScale(0.7f, 0.7f);
	collider->radius = 0.2;
	collider->isTrigger = true;
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
	if (other.actor->CompareTag("Player"))
	{
		other.actor->GetActorAs<Player>()->TakeDamage(3);
		Destroy();
	}

}
