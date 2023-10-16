#include "Actor.h"
#include "ActorComponent.h"
#include "Collider2D.h"
#include "Physics.h"
#include "PhysicsSystem.h"

noa::Cell* noa::Grid::GetCell(int x, int y)
{
	return &cells[y * width + x];
}

noa::Collider2D::Collider2D(Actor* actor, Rigidbody* rigidbody) :ActorComponent(actor)
{
	this->rigidbody = rigidbody;
	if (rigidbody!=nullptr)
	{
		rigidbody->BindCollider(this);
	}
	
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
		&& actor != nullptr;
	if (canApplyTrigger)
	{
		actor->OnTrigger(rigidbody->collision);
	}
	rigidbody->collision.actor = nullptr;

	if (isHitCollisionTile)
	{
		this->actor->OnHitTile();
		isHitCollisionTile = false;
	}

}

void noa::Collider2D::Update()
{
	if (rigidbody == nullptr)
	{
		return;
	}
	const int x = static_cast<int>(actor->transform.position.x);
	const int y = static_cast<int>(actor->transform.position.y);

	const int w = PhysicsSystem::grid.width;
	const int h = PhysicsSystem::grid.height;

	if (x < 0 || x >= w || y < 0 || y >= h)
	{
		return;
	}
	PhysicsSystem::grid.GetCell(x, y)->colliders.push_back(this);
}

void noa::Collider2D::SetTileMap(TileMap* tileMap)
{
	this->tileMap = tileMap;
}

noa::CircleCollider2D::CircleCollider2D(Actor* actor, Rigidbody* rigidbody):noa::Collider2D(actor,rigidbody)
{
	this->colliderType = ColliderType::CIRCLE_COLLIDER;
}

noa::CircleCollider2D::~CircleCollider2D()
{

}

noa::CircleCollider2D* noa::CircleCollider2D::Create(Actor* actor, Rigidbody* rigidbody)
{
	return new CircleCollider2D(actor, rigidbody);
}

noa::BoxCollider2D::BoxCollider2D(noa::Actor* actor, noa::Rigidbody* rigidbody) :Collider2D(actor,rigidbody)
{
	this->colliderType = noa::ColliderType::BOX_COLLIDER;
}

noa::BoxCollider2D::~BoxCollider2D()
{

}

noa::BoxCollider2D* noa::BoxCollider2D::Create(Actor * actor, Rigidbody * rigidbody)
{
	return new BoxCollider2D(actor,rigidbody);
}
