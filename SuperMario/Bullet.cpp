#include "Bullet.h"
#include "BulletPool.h"

using namespace noa;

Bullet::Bullet(Scene* scene):Actor(scene) 
{
	spriteRenderer->SetSprite(&sprite);
}

void Bullet::Update() {
	rigid->velocity.y = -10;
	time = time + Time::deltaTime;
	if (time >0.5)
	{
		bulletPool->Return(this);
		time = 0;
	}
}

void Bullet::SetPostion(const Vector<float>& pos)
{
	this->transform.position = pos;
}