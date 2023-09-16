#include "Enimy.h"

Enimy::Enimy(Sprite* sprite, Transform* player,LiveEntity * enimy) :
	GameObject(sprite)
	, Rigidbody(this)
{

	tag = "Enimy";

	gameObject = this;
	this->player = player;
	this->useGravity = false;
	this->enimy = enimy;

	fsm = new StateMachine();

}

Enimy::~Enimy()
{
	Rigidbody::~Rigidbody();
	GameObject::~GameObject();
}


void Enimy::Update()
{
	if (currentAnimation!=nullptr) 
	{
		sprite->UpdateImage(currentAnimation->GetCurrentFrameImage());
	}
	fsm->Reason();
	fsm->Act();
	
}
