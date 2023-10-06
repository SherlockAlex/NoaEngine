#include "Bullet.h"
#include "BulletPool.h"
#include "Player.h"

using namespace noa;

Bullet::Bullet(Scene* scene):Actor(scene) 
{
	spriteRenderer->SetSprite(&sprite);

	rigid->collision.isTrigger = true;
	rigid->tag = "Bullet";
	collider->radius = 0.2f;
}

void Bullet::SetDestroy()
{
	time = 0;
	bulletPool->Return(this);
}

void Bullet::Update() {
	rigid->velocity.y = -12;
	time = time + Time::deltaTime;

	if (time >0.5)
	{
		SetDestroy();
	}

}

void Bullet::SetPostion(const Vector<float>& pos)
{
	this->transform.position = pos;
}

void Bullet::OnTrigger(const noa::Collision& other)
{
	Rigidbody* rigidbody = other.other;
	if (rigidbody == nullptr)
	{
		return;
	}
	if (rigidbody->tag == this->ownTag || rigidbody->tag == this->rigid->tag)
	{
		return;
	}
	//�����¼�

	LiveEntity* entity = rigidbody->GetActorAs<LiveEntity*>();
	if (entity == nullptr) 
	{
		return;
	}
	entity->TakeDamage(20);
	SetDestroy();
}
