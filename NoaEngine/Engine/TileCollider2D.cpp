#include "TileCollider2D.h"
#include "Rigidbody.h"
#include "TileMap.h"

noa::TileCollider2D::TileCollider2D(Actor* actor)
	:noa::Collider2D(actor)
{
	this->colliderType = ColliderType::TILE_COLLIDER;
}

noa::TileCollider2D::~TileCollider2D()
{

}

bool noa::TileCollider2D::CheckCollision(float x,float y) 
{
	const int posX = static_cast<int>(x);
	const int posY = static_cast<int>(y);
	return tileMap->CheckCollision(posX,posY);
}

bool noa::TileCollider2D::CheckCollision(
	int x1, int y1
	, int x2, int y2
)
{
	const int posX1 = static_cast<int>(x1);
	const int posY1 = static_cast<int>(y1);

	const int posX2 = static_cast<int>(x2);
	const int posY2 = static_cast<int>(y2);
	return tileMap->CheckCollision(posX1, posY1)
		|| tileMap->CheckCollision(posX2,posY2);
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

noa::TileMap* noa::TileCollider2D::GetTileMap() {
	return this->tileMap;
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

noa::TileCollider2D& noa::TileCollider2D::SetTrigger(bool value)
{
	noa::Collider2D::SetTrigger(value);
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

	if (size.x* size.y == 0)
	{
		return;
	}
	
	Actor* actor = GetActor();

	float newX = rigidbody->newPosition.x;
	float newY = rigidbody->newPosition.y;
	float posX = actor->transform.position.x;
	float posY = actor->transform.position.y;

	int intNewX = static_cast<int>(newX);
	int intNewY = static_cast<int>(newY);
	int intPosX = static_cast<int>(posX);
	int intPosY = static_cast<int>(posY);

	float deltaStep = 0.9999f;
	float sizeX = deltaStep * std::abs(this->size.x);
	float sizeY = deltaStep * std::abs(this->size.y);

	//先应用冲量
	const float bounce = std::abs(rigidbody->bounce);


	if (CheckCollision(intNewX, intPosY
		, intNewX, static_cast<int>(posY + sizeY))
		)
	{
		this->isHitCollisionTile = true;
		rigidbody->velocity.x = (-rigidbody->velocity.x * bounce);
		if (static_cast<int>(rigidbody->velocity.x * 10.0f) == 0)
		{
			rigidbody->velocity.x = 0;
		}

		newX = intNewX + 1.0f;

		intNewX = static_cast<int>(newX);
	}

	if (CheckCollision(
		static_cast<int>(newX + sizeX), intPosY
		,static_cast<int>(newX + sizeX), static_cast<int>(posY + sizeY))
		)
	{
		this->isHitCollisionTile = true;
		rigidbody->velocity.x = (-rigidbody->velocity.x * bounce);
		if (static_cast<int>(rigidbody->velocity.x * 10.0f) == 0)
		{
			rigidbody->velocity.x = 0;
		}

		newX = static_cast<int>(newX + sizeX) - size.x;

		intNewX = static_cast<int>(newX);
	}

	if (CheckCollision(intNewX, intNewY
		,static_cast<int>(newX + sizeX), intNewY))
	{
		this->isHitCollisionTile = true;
		rigidbody->velocity.y = (-rigidbody->velocity.y * bounce);
		if (static_cast<int>(rigidbody->velocity.y * 10.0f) == 0)
		{
			rigidbody->velocity.y = 0;
		}

		newY = intNewY + 1.0f;

		intNewY = static_cast<int>(newY);

	}

	if (CheckCollision(intNewX, static_cast<int>(newY + sizeY)
	, static_cast<int>(newX + sizeX), static_cast<int>(newY + sizeY)))
	{
		//下
		this->isHitCollisionTile = true;
		rigidbody->velocity.y = (-rigidbody->velocity.y * bounce);
		if (static_cast<int>(rigidbody->velocity.y * 10.0f) == 0)
		{
			rigidbody->velocity.y = 0;
		}

		newY = static_cast<int>(newY + sizeY) - size.y;
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

	if (size.x*size.y == 0)
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
	float sizeX = deltaStep * std::abs(this->size.x);
	float sizeY = deltaStep * std::abs(this->size.y);

	if (CheckCollision(intNewX, intPosY, intNewX
		, static_cast<int>(posY + sizeY)))
	{
		//左
		this->isHitCollisionTile = true;

		if (static_cast<int>(rigidbody->velocity.x * 10.0f) == 0)
		{
			//rigidbody->velocity.x = 0;
			rigidbody->nextConstraint.x = true;
			rigidbody->constraint.x = true;
		}

		newX = intNewX + 1.0f;

		intNewX = static_cast<int>(newX);
	}

	if (CheckCollision(static_cast<int>(newX + sizeX), intPosY
		, static_cast<int>(newX + sizeX), static_cast<int>(posY + sizeY)))
	{
		//右
		this->isHitCollisionTile = true;

		if (static_cast<int>(rigidbody->velocity.x * 10.0f) == 0)
		{
			rigidbody->nextConstraint.x = true;
			rigidbody->constraint.x = true;
		}

		newX = static_cast<int>(newX + sizeX) - size.x;

		intNewX = static_cast<int>(newX);
	}

	if (CheckCollision(intNewX, intNewY
		, static_cast<int>(newX + sizeX), intNewY))
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

	if (CheckCollision(intNewX, static_cast<int>(newY + sizeY)
	, static_cast<int>(newX + sizeX), static_cast<int>(newY + sizeY)))
	{
		//下
		if (static_cast<int>(rigidbody->velocity.y * 10.0f) == 0)
		{
			rigidbody->nextConstraint.y = true;
			rigidbody->constraint.y = true;
			this->isGrounded = true;
		}

		newY = static_cast<int>(newY + sizeY) - size.y;
		intNewY = static_cast<int>(newY);

	}

}

noa::TileCollider2D& noa::TileCollider2D::SetSize(float x, float y)
{
	this->size.x = x;
	this->size.y = y;
	return *this;
}