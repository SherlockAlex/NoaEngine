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
		&& this->GetActor() != nullptr;
	if (canApplyTrigger)
	{
		this->GetActor()->OnTrigger(rigidbody->collision);
	}
	rigidbody->collision.actor = nullptr;

	

}

void noa::Collider2D::Update()
{
	if (rigidbody == nullptr)
	{
		return;
	}
	const int x = static_cast<int>(this->GetActor()->transform.position.x);
	const int y = static_cast<int>(this->GetActor()->transform.position.y);

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

noa::CircleCollider2D::CircleCollider2D(Actor* actor):noa::Collider2D(actor)
{
	this->colliderType = ColliderType::CIRCLE_COLLIDER;
}

noa::CircleCollider2D::~CircleCollider2D()
{

}

noa::CircleCollider2D* noa::CircleCollider2D::Create(Actor* actor)
{
	return NObject<CircleCollider2D>::Create<Actor*>(actor);
}

noa::CircleCollider2D& noa::CircleCollider2D::SetRadius(float radius)
{
	this->radius = radius;
	return *this;
}

noa::CircleCollider2D& noa::CircleCollider2D::SetRigidbody(noa::Rigidbody * rigidbody)
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

noa::BoxCollider2D::BoxCollider2D(noa::Actor* actor) :Collider2D(actor)
{
	this->colliderType = noa::ColliderType::BOX_COLLIDER;
}

noa::BoxCollider2D::~BoxCollider2D()
{

}

noa::BoxCollider2D* noa::BoxCollider2D::Create(Actor * actor)
{
	return NObject<BoxCollider2D>::Create<Actor*>(actor);
}

noa::BoxCollider2D& noa::BoxCollider2D::SetScale(int x, int y)
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

noa::TileCollider2D::TileCollider2D(Actor* actor)
	:noa::Collider2D(actor)
{
	this->colliderType = ColliderType::TILE_COLLIDER;
	if (actor)
	{
		this->SetTileMap(actor->GetActiveScene()->GetLevelAs<TileMap>());
	}
}

noa::TileCollider2D::~TileCollider2D()
{

}

noa::TileCollider2D* noa::TileCollider2D::Create(noa::Actor* actor)
{
	TileCollider2D * collider = 
		noa::NObject<TileCollider2D>::Create<noa::Actor*>(actor);
	return collider;
}

void noa::TileCollider2D::Update()
{
	//他只会检测Rigidbody是否和TileMap发生了碰撞
	//不会去检测其他
	if (isHitCollisionTile)
	{
		this->GetActor()->OnHitTile();
		isHitCollisionTile = false;
	}
}

void noa::TileCollider2D::LateUpdate()
{
	this->isGrounded = false;
}

noa::TileCollider2D& noa::TileCollider2D::SetTileMap(TileMap* tileMap)
{
	this->tileMap = tileMap;
	return *this;
}

noa::TileCollider2D& noa::TileCollider2D::SetRigidbody(Rigidbody * rigidbody) 
{
	Collider2D::SetRigidbody(rigidbody);
	if (rigidbody)
	{
		rigidbody->tileCollider2D = this;
	}
	return *this;
}

noa::TileCollider2D* noa::TileCollider2D::Apply()
{
	return this;
}

void noa::TileCollider2D::ApplyTileCollision(float deltaTime)
{
	if (!GetActive() || tileMap == nullptr || rigidbody == nullptr)
	{
		return;
	}

	if (scale.x == 0 || scale.y == 0)
	{
		return;
	}

	float newX = rigidbody->newPosition.x;
	float newY = rigidbody->newPosition.y;
	float posX = GetActor()->transform.position.x;
	float posY = GetActor()->transform.position.y;

	int intNewX = static_cast<int>(newX);
	int intNewY = static_cast<int>(newY);
	int intPosX = static_cast<int>(posX);
	int intPosY = static_cast<int>(posY);

	float deltaStep = 0.999f;
	float scaleX = deltaStep * std::abs(this->scale.x);
	float scaleY = deltaStep * std::abs(this->scale.y);

	//先应用冲量

	if (tileMap->IsCollisionTile(intNewX, intPosY)
		|| tileMap->IsCollisionTile(intNewX, static_cast<int>(posY + scaleY))
		)
	{
		rigidbody->velocity.x = -rigidbody->velocity.x * std::abs(rigidbody->friction);
		if (static_cast<int>(rigidbody->velocity.x*10.0f) == 0)
		{
			rigidbody->velocity.x = 0;
		}

		newX = intNewX + 1.0f;
		
		intNewX = static_cast<int>(newX);
	}

	if (tileMap->IsCollisionTile(static_cast<int>(newX + scaleX), intPosY)
		|| tileMap->IsCollisionTile(static_cast<int>(newX + scaleX), static_cast<int>(posY + scaleY))
		)
	{

		rigidbody->velocity.x = -rigidbody->velocity.x * std::abs(rigidbody->friction);
		if (static_cast<int>(rigidbody->velocity.x * 10.0f) == 0)
		{
			rigidbody->velocity.x = 0;
		}

		newX = static_cast<int>(newX + scaleX) - scale.x;
		
		intNewX = static_cast<int>(newX);
	}

	if (tileMap->IsCollisionTile(intNewX, intNewY)
		|| tileMap->IsCollisionTile(static_cast<int>(newX + scaleX), intNewY))
	{
		
		rigidbody->velocity.y = -rigidbody->velocity.y * std::abs(rigidbody->bounce);
		if (static_cast<int>(rigidbody->velocity.y * 10.0f) == 0)
		{
			rigidbody->velocity.y = 0;
		}

		newY = intNewY + 1.0f;
		
		intNewY = static_cast<int>(newY);

	}

	if (tileMap->IsCollisionTile(intNewX, static_cast<int>(newY + scaleY))
		|| tileMap->IsCollisionTile(static_cast<int>(newX + scaleX), static_cast<int>(newY + scaleY)))
	{
		//下
		
		rigidbody->velocity.y = -rigidbody->velocity.y * std::abs(rigidbody->bounce);
		if (static_cast<int>(rigidbody->velocity.y * 10.0f) == 0)
		{
			rigidbody->velocity.y = 0;
		}

		newY = static_cast<int>(newY + scaleY) - scale.y;
		intNewY = static_cast<int>(newY);

	}

	rigidbody->newPosition.x = newX;
	rigidbody->newPosition.y = newY;
}

void noa::TileCollider2D::ApplyConstraint(float deltaTime)
{
	if (!GetActive() || tileMap == nullptr || rigidbody == nullptr)
	{
		return;
	}

	if (scale.x == 0 || scale.y == 0)
	{
		return;
	}

	float newX = rigidbody->newPosition.x;
	float newY = rigidbody->newPosition.y;
	float posX = GetActor()->transform.position.x;
	float posY = GetActor()->transform.position.y;

	int intNewX = static_cast<int>(newX);
	int intNewY = static_cast<int>(newY);
	int intPosX = static_cast<int>(posX);
	int intPosY = static_cast<int>(posY);

	float deltaStep = 1.01f;
	float scaleX = deltaStep * std::abs(this->scale.x);
	float scaleY = deltaStep * std::abs(this->scale.y);

	if (tileMap->IsCollisionTile(intNewX, intPosY)
		|| tileMap->IsCollisionTile(intNewX, static_cast<int>(posY + scaleY))
		)
	{
		//左
		this->isHitCollisionTile = true;

		if (static_cast<int>(rigidbody->velocity.x * 10.0f) == 0)
		{
			rigidbody->nextConstraint.x = true;
			rigidbody->constraint.x = true;
		}

		newX = intNewX + 1.0f;

		intNewX = static_cast<int>(newX);
	}

	if (tileMap->IsCollisionTile(static_cast<int>(newX + scaleX), intPosY)
		|| tileMap->IsCollisionTile(static_cast<int>(newX + scaleX), static_cast<int>(posY + scaleY))
		)
	{
		//右
		this->isHitCollisionTile = true;

		if (static_cast<int>(rigidbody->velocity.x * 10.0f) == 0)
		{
			rigidbody->nextConstraint.x = true;
			rigidbody->constraint.x = true;
		}

		newX = static_cast<int>(newX + scaleX) - scale.x;

		intNewX = static_cast<int>(newX);
	}

	if (tileMap->IsCollisionTile(intNewX, intNewY)
		|| tileMap->IsCollisionTile(static_cast<int>(newX + scaleX), intNewY))
	{
		//上
		this->isHitCollisionTile = true;

		if (static_cast<int>(rigidbody->velocity.y * 10.0f) == 0)
		{
			rigidbody->nextConstraint.y = true;
			rigidbody->constraint.y = true;
		}

		newY = intNewY + 1.0f;

		intNewY = static_cast<int>(newY);

	}

	if (tileMap->IsCollisionTile(intNewX, static_cast<int>(newY + scaleY))
		|| tileMap->IsCollisionTile(static_cast<int>(newX + scaleX), static_cast<int>(newY + scaleY)))
	{
		//下
		if (static_cast<int>(rigidbody->velocity.y * 10.0f) == 0)
		{
			rigidbody->nextConstraint.y = true;
			rigidbody->constraint.y = true;
			this->isGrounded = true;
		}

		newY = static_cast<int>(newY + scaleY) - scale.y;
		intNewY = static_cast<int>(newY);

	}

}

noa::TileCollider2D& noa::TileCollider2D::SetScale(float x, float y)
{
	this->scale.x = x;
	this->scale.y = y;
	return *this;
}