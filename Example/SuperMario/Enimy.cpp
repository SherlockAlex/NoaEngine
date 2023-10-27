#include "Enimy.h"
#include "Bullet.h"
#include "BulletPool.h"
using namespace noa;

Sprite enimySprite(noa::Resource::LoadSprite("./Assets/Fly/enimy.spr"), { 32,32 });


Enimy::Enimy(noa::Scene* scene) :LiveEntity(scene)
{
	rigid->useGravity = true;
	rigid->gravityWeight = 3.5f;
	rigid->damping = 0.0f;
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
	
	//rigid->velocity.y = 0.1f;

	delay = delay + Time::deltaTime;
	if (delay > 0.5)
	{
		Bullet* bullet = bulletPool->Request();

		bullet->SetDirection(sceneManager.FindActorWithTag("Player")->transform.position - transform.position);
		bullet->ownTag = tag;
		bullet->SetPostion(this->transform.position);
		delay = 0;
	}

	if (rigid->velocity.y>0&&this->transform.position.y>2.5f) 
	{
		rigid->velocity.y = 0;
		this->transform.position.y = 2.5f;
	}

}
