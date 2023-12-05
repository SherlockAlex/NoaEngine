#include "Actor.h"
#include "ActorComponent.h"
#include "Collider2D.h"
#include "Rigidbody.h"
#include "PhysicsSystem.h"
#include "Scene.h"

noa::Cell* noa::Grid::GetCell(int x, int y)
{
	return &cells[y * width + x];
}

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

	const Vector<float> position = this->GetActor()->transform.position;
	const int x = static_cast<int>(position.x);
	const int y = static_cast<int>(position.y);

	const int w = PhysicsSystem::grid.width;
	const int h = PhysicsSystem::grid.height;

	if (x < 0 || x >= w || y < 0 || y >= h)
	{
		return;
	}
	PhysicsSystem::grid.GetCell(x, y)->colliders.push_back(this);
	
	
	
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