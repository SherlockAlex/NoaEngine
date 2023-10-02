#include "Collider2D.h"
#include "PhysicsSystem.h"

noa::Cell& noa::Grid::GetCell(int x, int y)
{
	// TODO: �ڴ˴����� return ���
	return cells[y * width + x];
}

noa::Collider2D::Collider2D(Actor* actor, Rigidbody* rigidbody) :ActorComponent(actor)
{
	this->rigidbody = rigidbody;
	
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
	if (rigidbody == nullptr)
	{
		return;
	}
	//��rigidbody�����Ӧ��cell����
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

void noa::CircleCollider2D::Delete()
{
	delete this;
}
