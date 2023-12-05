#include "Actor.h"
#include "ActorComponent.h"
#include "Collider2D.h"
#include "Rigidbody.h"
#include "PhysicsSystem.h"
#include "Scene.h"

noa::Collider2D::Collider2D(Actor* actor) :ActorComponent(actor)
{

}

noa::Collider2D::~Collider2D()
{

}

void noa::Collider2D::ApplyTriggerEnter(Collider2D& other)
{

	Actor* actorHandled = GetActor();
	const bool canApplyTrigger = GetActive()
		&& isTrigger
		&& actorHandled != nullptr;
	if (canApplyTrigger)
	{
		actorHandled->OnTriggerEnter(other);
	}

}

void noa::Collider2D::Update()
{
	
	if (rigidbody == nullptr)
	{
		return;
	}
	noa::PhysicsSystem::colliders.push_back(this);
	
}

void noa::Collider2D::SetRigidbody(Rigidbody* rigidbody)
{
	this->rigidbody = rigidbody;
	if (rigidbody != nullptr)
	{
		rigidbody->BindCollider(this);
	}
}

void noa::Collider2D::SetTrigger(bool value)
{
	this->isTrigger = value;
}