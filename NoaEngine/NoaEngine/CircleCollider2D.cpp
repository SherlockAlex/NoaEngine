#include "CircleCollider2D.h"

noa::CircleCollider2D::CircleCollider2D(Actor* actor) :noa::Collider2D(actor)
{
	this->colliderType = ColliderType::CIRCLE_COLLIDER;
}

noa::CircleCollider2D::~CircleCollider2D()
{

}

noa::CircleCollider2D* noa::CircleCollider2D::Create(Actor* actor)
{
	return NObject<CircleCollider2D>::Create(actor);
}

noa::CircleCollider2D& noa::CircleCollider2D::SetRadius(float radius)
{
	this->radius = radius;
	return *this;
}

noa::CircleCollider2D& noa::CircleCollider2D::SetRigidbody(noa::Rigidbody* rigidbody)
{
	Collider2D::SetRigidbody(rigidbody);
	return *this;
}

noa::CircleCollider2D& noa::CircleCollider2D::SetIsTrigger(bool isTrigger)
{
	this->isTrigger = isTrigger;
	return *this;
}

noa::CircleCollider2D* noa::CircleCollider2D::Apply()
{
	return this;
}