#include "Bullet.h"
#include "Player.h"
#include "WolfResource.h"

Bullet::Bullet(Scene * scene,Sprite* sprite) :GameObject(scene,sprite, {32,32})
{
	animation->SetFrame(&wolfResource->bulletDestroyFrame);

	rigid->collision.sacle = { -0.3,-0.3 };
	rigid->collision.radius = 0.2;
	rigid->collision.isTrigger = true;
	rigid->useGravity = false;
	rigid->tag = "Bullet";
	
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

void Bullet::Delete()
{
	delete this;
}

Bullet::~Bullet()
{
	Debug("Remove bullet");
}

void Bullet::Update()
{
	rigid->velocity = dir.Normalize() * speed;
	this->sprite->UpdateImage(animation->GetCurrentFrameImage());
}

void Bullet::OnHitTile()
{
	//rigid->Destroy();
	rigid->Destroy();
	animation->Play();
	
}

void Bullet::OnTrigger(Collision other)
{
	Rigidbody* rigid = other.other;

	Player* player = rigid->GetActorAs<Player*>();
	if (player!=nullptr&&rigid->tag == "Player")
	{
		//this->rigid->Destroy();
		player->TakeDamage(3);
		Destroy();
	}

}
