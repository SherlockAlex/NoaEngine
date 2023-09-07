#include "Enimy.h"

Enimy::Enimy(Sprite sprite, Transform* player,LiveEntity * enimy) :
	GameObject(new Sprite(sprite))
	, Rigidbody(&transform)
{

	tag = "Enimy";

	gameObject = this;
	this->player = player;
	this->useGravity;
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

	fsm->Act();
	fsm->Reason();
}