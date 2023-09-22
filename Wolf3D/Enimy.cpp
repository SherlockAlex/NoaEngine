#include "Enimy.h"

Enimy::Enimy(Scene* scene, Sprite* sprite, Transform* player, LiveEntity* enimy) :
	GameObject(scene,sprite)
{

	rigid->tag = "Enimy";

	this->player = player;
	rigid->useGravity = false;
	this->enimy = enimy;

}

Enimy::~Enimy()
{

	//Rigidbody::~Rigidbody();
	GameObject::~GameObject();
	
	Debug("Remove enimy");
}


void Enimy::Update()
{
	if (currentAnimation!=nullptr) 
	{
		sprite->UpdateImage(currentAnimation->GetCurrentFrameImage());
	}
	fsm.Reason();
	fsm.Act();
	
}

void Enimy::OnDestroy()
{
	//fsm->Delete();
}
