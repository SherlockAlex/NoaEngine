#include "Enimy.h"
using namespace noa;

Sprite enimySprite(noa::resource.LoadSprFile("./Assets/Fly/player.spr"), { 32,32 });


Enimy::Enimy(noa::Scene* scene) :LiveEntity(scene)
{
	rigid->useGravity = false;
	collider->radius = 0.5f;
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
