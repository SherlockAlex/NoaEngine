#include "Bullet.h"
#include "BulletPool.h"
#include "ResourceManager.h"

Bullet::Bullet(noa::Scene* scene) :noa::Actor(scene) {

}

Bullet::~Bullet() {

}

void Bullet::Start() 
{

}

void Bullet::OnEnable()
{
}

void Bullet::Update() {
	this->rigid->velocity = this->direction * this->speed;

	timer += noa::Time::deltaTime;
	if (timer>1.0f)
	{
		bulletPool->Return(this);
	}
}

void Bullet::OnDisable()
{
	this->timer = 0;
}

void Bullet::OnHitTile() 
{
	bulletPool->Return(this);
}

void Bullet::OnTrigger(const noa::Collision & collision) 
{
	if (collision.actor!=nullptr)
	{
		noa::Debug::Log(collision.actor->tag);
	}
}

void Bullet::SetDirection(float x,float y)
{
	this->direction.x = x;
	this->direction.y = y;
	this->direction = this->direction.Normalize();
}
