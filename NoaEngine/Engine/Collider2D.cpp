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

void noa::Collider2D::UpdateCaculateVertices(){
	if (rigidbody == nullptr)
	{
		return;
	}

	const size_t count = caculateVertices.size();
	const noa::Vector<float> position = rigidbody->newPosition + offset;
	
	Actor* actor = GetActor();
	const float cos = cosf(actor->transform.eulerAngle);
	const float sin = sinf(actor->transform.eulerAngle);
	
	for (size_t i = 0; i < count;i++)
	{
		
		noa::Vector<float> result;
		result.x = cos * vertices[i].x - sin * vertices[i].y;
		result.y = sin * vertices[i].x + cos * vertices[i].y;
		caculateVertices[i] = result + position;
	}

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

noa::Rigidbody* noa::Collider2D::GetRigidbody() {
	return rigidbody;
}