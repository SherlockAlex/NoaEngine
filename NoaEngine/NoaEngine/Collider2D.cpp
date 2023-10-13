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

void noa::Collider2D::ApplyTileMapCollision()
{

	if (!GetActive() || rigidbody == nullptr || !rigidbody->useCollision || tileMap == nullptr)
	{
		return;
	}

	const float scaleX = rigidbody->collision.sacle.x;
	const float scaleY = rigidbody->collision.sacle.y;

	if (tileMap->IsCollisionTile(static_cast<int>(rigidbody->newPosition.x - scaleX), static_cast<int>(this->actor->transform.position.y - scaleY))
		|| tileMap->IsCollisionTile(static_cast<int>(rigidbody->newPosition.x - scaleX), static_cast<int>(this->actor->transform.position.y + 0.999 + scaleY))
	)
	{
		isHitCollisionTile = true;
		if (!isTrigger)
		{

			rigidbody->newPosition.x = (int)rigidbody->newPosition.x + 1 + scaleX;
			rigidbody->velocity.x = 0;
		}
	}

	if (tileMap->IsCollisionTile(static_cast<int>(rigidbody->newPosition.x + 0.999 + scaleX), static_cast<int>(this->actor->transform.position.y - scaleY))
		|| tileMap->IsCollisionTile(static_cast<int>(rigidbody->newPosition.x + 0.999 + scaleX), static_cast<int>(this->actor->transform.position.y + 0.999 + scaleY))
	)
	{
		isHitCollisionTile = true;
		if (!isTrigger)
		{

			rigidbody->newPosition.x = (int)rigidbody->newPosition.x - scaleX;
			rigidbody->velocity.x = 0;
		}

	}

	if (tileMap->IsCollisionTile(static_cast<int>(rigidbody->newPosition.x - scaleX), static_cast<int>(rigidbody->newPosition.y - scaleY))
		|| tileMap->IsCollisionTile(static_cast<int>(rigidbody->newPosition.x + 0.999 + scaleX), static_cast<int>(rigidbody->newPosition.y - scaleY))
	)
	{
		isHitCollisionTile = true;
		if (!isTrigger)
		{

			rigidbody->newPosition.y = (int)rigidbody->newPosition.y + 1 + scaleY;
			rigidbody->velocity.y = 0;
		}
	}

	if (tileMap->IsCollisionTile(static_cast<int>(rigidbody->newPosition.x - scaleX), static_cast<int>(rigidbody->newPosition.y + 0.999 + scaleY))
		|| tileMap->IsCollisionTile(static_cast<int>(rigidbody->newPosition.x + 0.999 + scaleX), static_cast<int>(rigidbody->newPosition.y + 0.999 + scaleY))
	)
	{
		isHitCollisionTile = true;
		if (!isTrigger) {
			rigidbody->collision.isGrounded = true;

			rigidbody->newPosition.y = (int)rigidbody->newPosition.y - scaleY;
			rigidbody->velocity.y = 0;
		}
	}

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
		&& rigidbody->collision.other != nullptr
		&& actor != nullptr;
	if (canApplyTrigger)
	{
		actor->OnTrigger(rigidbody->collision);
	}
	rigidbody->collision.other = nullptr;

	if (isHitCollisionTile)
	{
		this->actor->OnHitTile();
		isHitCollisionTile = false;
	}

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
