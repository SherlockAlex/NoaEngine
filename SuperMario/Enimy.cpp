#include "Enimy.h"
#include "Bullet.h"
#include "BulletPool.h"
using namespace noa;

Sprite enimySprite(noa::resource.LoadSprFile("./Assets/Fly/enimy.spr"), { 32,32 });


Enimy::Enimy(noa::Scene* scene) :LiveEntity(scene)
{
	rigid->useGravity = false;
	this->spriteRenderer->SetSprite(&enimySprite);
}

void Enimy::TakeDamage(int damage)
{
	hp -= damage;
	if (hp<=0) 
	{
		Debug::Log("Enimy is done");
		Destroy();
	}
}

void Enimy::SetPosition(const Vector<float>& position)
{
	this->transform.position = position;
}

void Enimy::Update() 
{
	
	rigid->velocity.y = 0.1f;

	delay = delay + Time::deltaTime;
	if (delay > 0.5)
	{
		//shootAFX.Play(false);
		Bullet* bullet = bulletPool->Request();
		bullet->SetDirection({ 0,1 });
		bullet->ownTag = rigid->tag;
		bullet->SetPostion(this->transform.position);
		delay = 0;
	}

}
