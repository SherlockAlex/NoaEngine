#include "TileCollider2D.h"
#include "Physics.h"

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
	TileCollider2D* collider =
		noa::NObject<TileCollider2D>::Create(actor);
	return collider;
}

void noa::TileCollider2D::Update()
{
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

noa::TileCollider2D& noa::TileCollider2D::SetRigidbody(Rigidbody* rigidbody)
{
	Collider2D::SetRigidbody(rigidbody);
	if (rigidbody)
	{
		rigidbody->tileCollider2D = this;
	}
	return *this;
}

noa::TileCollider2D& noa::TileCollider2D::SetIsTrigger(bool value)
{
	this->isTrigger = value;
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
		this->isHitCollisionTile = true;
		rigidbody->velocity.x = -rigidbody->velocity.x * std::abs(rigidbody->friction);
		if (static_cast<int>(rigidbody->velocity.x * 10.0f) == 0)
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
		this->isHitCollisionTile = true;
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
		this->isHitCollisionTile = true;
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
		this->isHitCollisionTile = true;
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
			rigidbody->velocity.x = 0;
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
			rigidbody->velocity.x = 0;
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
			rigidbody->velocity.y = 0;
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
			rigidbody->velocity.y = 0;
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