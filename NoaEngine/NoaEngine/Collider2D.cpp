#include "Actor.h"
#include "ActorComponent.h"
#include "Collider2D.h"
#include "Physics.h"
#include "PhysicsSystem.h"
#include "Scene.h"

noa::Cell* noa::Grid::GetCell(int x, int y)
{
	return &cells[y * width + x];
}

noa::Collider2D::Collider2D(Actor* actor) :ActorComponent(actor)
{
	this->colliderType = ColliderType::TILE_COLLIDER;
}

noa::Collider2D::~Collider2D()
{

}

void noa::Collider2D::ApplyTrigger()
{
	if (rigidbody == nullptr)
	{
		return;
	}

	const bool canApplyTrigger =  rigidbody->GetActive()
		&& GetActive()
		&& isTrigger
		&& rigidbody->collision.actor != nullptr
		&& GetActor() != nullptr;
	if (canApplyTrigger)
	{
		GetActor()->OnTrigger(rigidbody->collision);
	}
	rigidbody->collision.actor = nullptr;

	

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
