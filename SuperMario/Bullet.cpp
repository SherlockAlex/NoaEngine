#include "Bullet.h"

using namespace noa;

Bullet::Bullet(Scene* scene):Actor(scene) 
{
	spriteRenderer->SetSprite(&sprite);
}


void Bullet::Start() {
	Debug::Log("Bullet Start");
}

void Bullet::Update() {
	rigid->velocity.y = -10;
	time = time + Time::deltaTime;
	if (time > 0.52)
	{
		Destroy();
	}
}

void Bullet::SetPostion(const Vector<float>& pos)
{
	this->transform.position = pos;
}