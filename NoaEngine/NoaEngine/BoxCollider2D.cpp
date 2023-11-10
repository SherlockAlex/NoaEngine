#include "BoxCollider2D.h"

noa::BoxCollider2D::BoxCollider2D(noa::Actor* actor) :Collider2D(actor)
{
	this->colliderType = noa::ColliderType::BOX_COLLIDER;
}

noa::BoxCollider2D::~BoxCollider2D()
{

}

noa::BoxCollider2D* noa::BoxCollider2D::Create(Actor* actor)
{
	return NObject<BoxCollider2D>::Create(actor);
}

noa::BoxCollider2D& noa::BoxCollider2D::SetScale(float x, float y)
{
	this->scale.x = x;
	this->scale.y = y;
	return *this;
}

noa::BoxCollider2D& noa::BoxCollider2D::SetIsTrigger(bool isTrigger)
{
	this->isTrigger = isTrigger;
	return *this;
}

noa::BoxCollider2D& noa::BoxCollider2D::SetRigidbody(Rigidbody* rigidbody)
{
	Collider2D::SetRigidbody(rigidbody);
	return *this;
}

noa::BoxCollider2D* noa::BoxCollider2D::Apply()
{
	return this;
}