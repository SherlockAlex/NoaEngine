#include "BoxCollider2D.h"
#include "Rigidbody.h"

noa::BoxCollider2D::BoxCollider2D(noa::Actor* actor) :Collider2D(actor)
{
	this->colliderType = noa::ColliderType::POLYGON_COLLIDER;
	this->vertices = { {}, {}, {}, {} };
	this->caculateVertices = { {}, {}, {}, {} };
}

noa::BoxCollider2D::~BoxCollider2D()
{

}

noa::BoxCollider2D* noa::BoxCollider2D::Create(Actor* actor)
{
	return NObject<BoxCollider2D>::Create(actor);
}

noa::BoxCollider2D& noa::BoxCollider2D::SetSize(float x, float y)
{
	this->size.x = x;
	this->size.y = y;

	vertices[0] = { 0,0 };
	vertices[1] = { x,0 };
	vertices[2] = { x,y };
	vertices[3] = { 0,y };

	return *this;
}

noa::BoxCollider2D& noa::BoxCollider2D::SetTrigger(bool value)
{
	Collider2D::SetTrigger(value);
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