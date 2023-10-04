#include "NoaEngine.h"

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

noa::Collider2D* noa::Collider2D::Create(Actor* actor, Rigidbody* rigidbody)
{
	return new Collider2D(actor,rigidbody);
}

void noa::Collider2D::Update()
{
	
}

void noa::Collider2D::Delete()
{
	delete this;
}

noa::CircleCollider2D::CircleCollider2D(Actor* actor, Rigidbody* rigidbody):noa::Collider2D(actor,rigidbody)
{

}

noa::CircleCollider2D::~CircleCollider2D()
{

}

noa::CircleCollider2D* noa::CircleCollider2D::Create(Actor* actor, Rigidbody* rigidbody)
{
	return new CircleCollider2D(actor, rigidbody);
}

void noa::CircleCollider2D::Update()
{
	if (rigidbody == nullptr)
	{
		return;
	}
	const int x = static_cast<int>(actor->transform.position.x+0.5);
	const int y = static_cast<int>(actor->transform.position.y+0.5);
	PhysicsSystem::grid.GetCell(x, y)->colliders.push_back(this);
}

void noa::CircleCollider2D::Delete()
{
	delete this;
}
