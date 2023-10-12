#include "Bullet.h"
#include "BulletPool.h"
#include "Player.h"

using namespace noa;

Bullet::Bullet(Scene* scene):Actor(scene) 
{
	spriteRenderer->SetSprite(&sprite);
	rigid->useGravity = true;
	rigid->collision.isTrigger = true;
	tag = "Bullet";
	collider->radius = 0.5f;
}

void Bullet::SetDirection(const noa::Vector<float>& direction)
{
	this->dir = direction;
}

void Bullet::SetDestroy()
{
	time = 0;
	bulletPool->Return(this);
}

void Bullet::Update() {

	rigid->velocity = dir * speed;

	time = time + Time::deltaTime;

	if (time >0.5)
	{
		SetDestroy();
	}

	if (rigid->velocity.y>0&&transform.position.y>4.5f) 
	{
		transform.position.y = 4.5f;
		rigid->velocity.y = 0;
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
	if (rigidbody->GetActorTag() == this->ownTag || rigidbody->GetActorTag() == this->rigid->GetActorTag())
	{
		return;
	}
	//处理事件

	LiveEntity* entity = rigidbody->GetActorAs<LiveEntity*>();
	if (entity == nullptr) 
	{
		return;
	}
	entity->TakeDamage(20);
	SetDestroy();
}
