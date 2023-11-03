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

noa::Collider2D::Collider2D(Actor* actor, Rigidbody* rigidbody) :ActorComponent(actor)
{
	this->colliderType = ColliderType::TILE_COLLIDER;
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

noa::CircleCollider2D::CircleCollider2D(Actor* actor, Rigidbody* rigidbody):noa::Collider2D(actor,rigidbody)
{
	this->colliderType = ColliderType::CIRCLE_COLLIDER;
}

noa::CircleCollider2D::~CircleCollider2D()
{

}

noa::CircleCollider2D* noa::CircleCollider2D::Create(Actor* actor, Rigidbody* rigidbody)
{
	return NObject<CircleCollider2D>::Create<Actor*, Rigidbody*>(actor, rigidbody);
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
	return NObject<BoxCollider2D>::Create<Actor*, Rigidbody*>(actor, rigidbody);
}

noa::TileCollider2D::TileCollider2D(Actor* actor,Rigidbody * rigidbody)
	:noa::Collider2D(actor,rigidbody)
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

noa::TileCollider2D* noa::TileCollider2D::Create(noa::Actor* actor, noa::Rigidbody* rigidbody)
{
	TileCollider2D * collider = 
		noa::NObject<TileCollider2D>::Create<noa::Actor*, noa::Rigidbody*>(actor, rigidbody);
	
	collider->SetTileMap(actor->GetActiveScene()->GetLevelAs<noa::TileMap>());
	rigidbody->tileCollider2D = collider;

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

void noa::TileCollider2D::SetTileMap(TileMap* tileMap)
{
	this->tileMap = tileMap;
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

	if (tileMap->IsCollisionTile(intNewX, intPosY)
		|| tileMap->IsCollisionTile(intNewX, static_cast<int>(posY + scaleY))
		)
	{
		//左
		this->isHitCollisionTile = true;
		
		rigidbody->velocity.x = -rigidbody->velocity.x * std::abs(1-friction);
		if (static_cast<int>(rigidbody->velocity.x) == 0)
		{
			rigidbody->velocity.x = 0;
			
		}

		this->rigidbody->momentum.x = this->rigidbody->velocity.x * this->rigidbody->mass;


		newX = intNewX + 1.0f;
		intNewX = static_cast<int>(newX);
	}

	if (tileMap->IsCollisionTile(static_cast<int>(newX + scaleX), intPosY)
		|| tileMap->IsCollisionTile(static_cast<int>(newX + scaleX), static_cast<int>(posY + scaleY))
		)
	{
		//右
		this->isHitCollisionTile = true;

		rigidbody->velocity.x = -rigidbody->velocity.x * std::abs(1-friction);
		if (static_cast<int>(rigidbody->velocity.x) == 0)
		{
			rigidbody->velocity.x = 0;
			
		}

		this->rigidbody->momentum.x = this->rigidbody->velocity.x * this->rigidbody->mass;

		newX = static_cast<int>(newX + scaleX) - scale.x;
		intNewX = static_cast<int>(newX);
	}

	if (tileMap->IsCollisionTile(intNewX, intNewY)
		|| tileMap->IsCollisionTile(static_cast<int>(newX + scaleX), intNewY))
	{
		//上
		this->isHitCollisionTile = true;
		
		rigidbody->velocity.y = -rigidbody->velocity.y * std::abs(1 - bounce);
		if (static_cast<int>(rigidbody->velocity.y) == 0)
		{
			rigidbody->velocity.y = 0;
		}

		this->rigidbody->momentum.y = this->rigidbody->velocity.y * this->rigidbody->mass;
		
		newY = intNewY + 1.0f;
		intNewY = static_cast<int>(newY);

	}

	if (tileMap->IsCollisionTile(intNewX, static_cast<int>(newY + scaleY))
		|| tileMap->IsCollisionTile(static_cast<int>(newX + scaleX), static_cast<int>(newY + scaleY)))
	{
		//下
		this->isGrounded = true;
		rigidbody->velocity.y = -rigidbody->velocity.y * std::abs(1 - bounce);
		if (static_cast<int>(rigidbody->velocity.y) == 0)
		{
			rigidbody->velocity.y = 0;
			
		}

		this->rigidbody->momentum.y = this->rigidbody->velocity.y * this->rigidbody->mass;
		
		newY = static_cast<int>(newY + scaleY) - scale.y;
		intNewY = static_cast<int>(newY);

	}

	rigidbody->newPosition.x = newX;
	rigidbody->newPosition.y = newY;
}

void noa::TileCollider2D::SetScale(float x, float y)
{
	this->scale.x = x;
	this->scale.y = y;
}

void noa::TileCollider2D::FixBodyPosition()
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

	if (tileMap->IsCollisionTile(intNewX, intPosY)
		|| tileMap->IsCollisionTile(intNewX, static_cast<int>(posY + scaleY))
		)
	{
		//左
		this->isHitCollisionTile = true;
		newX = intNewX + 1.0f;
		intNewX = static_cast<int>(newX);
	}

	if (tileMap->IsCollisionTile(static_cast<int>(newX + scaleX), intPosY)
		|| tileMap->IsCollisionTile(static_cast<int>(newX + scaleX), static_cast<int>(posY + scaleY))
		)
	{
		//右
		this->isHitCollisionTile = true;
		newX = static_cast<int>(newX + scaleX) - scale.x;
		intNewX = static_cast<int>(newX);
	}

	if (tileMap->IsCollisionTile(intNewX, intNewY)
		|| tileMap->IsCollisionTile(static_cast<int>(newX + scaleX), intNewY))
	{
		//上
		this->isHitCollisionTile = true;
		newY = intNewY + 1.0f;
		intNewY = static_cast<int>(newY);

	}

	if (tileMap->IsCollisionTile(intNewX, static_cast<int>(newY + scaleY))
		|| tileMap->IsCollisionTile(static_cast<int>(newX + scaleX), static_cast<int>(newY + scaleY)))
	{
		//下
		this->isHitCollisionTile = true;
		isGrounded = true;
		newY = static_cast<int>(newY + scaleY) - scale.y;
		intNewY = static_cast<int>(newY);

	}

	rigidbody->newPosition.x = newX;
	rigidbody->newPosition.y = newY;
}

void noa::TileCollider2D::FixBodyVelocity()
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

	if (tileMap->IsCollisionTile(intNewX, intPosY)
		|| tileMap->IsCollisionTile(intNewX, static_cast<int>(posY + scaleY))
		)
	{
		//左
		//this->isHitCollisionTile = true;
		rigidbody->velocity.x = -rigidbody->velocity.x * std::abs(1 - friction);

		if (static_cast<int>(std::abs(rigidbody->velocity.x)) == 0)
		{
			rigidbody->velocity.x = 0;
		}

		//rigidbody->newVelocity.x = -rigidbody->newVelocity.x;
		newX = intNewX + 1.0f;
		intNewX = static_cast<int>(newX);
	}

	if (tileMap->IsCollisionTile(static_cast<int>(newX + scaleX), intPosY)
		|| tileMap->IsCollisionTile(static_cast<int>(newX + scaleX), static_cast<int>(posY + scaleY))
		)
	{
		//右
		//this->isHitCollisionTile = true;
		rigidbody->velocity.x = -rigidbody->velocity.x * std::abs(1 - friction);
		if (static_cast<int>(std::abs(rigidbody->velocity.x)) == 0)
		{
			rigidbody->velocity.x = 0;
		}
		//rigidbody->newVelocity.x = -rigidbody->newVelocity.x;
		newX = static_cast<int>(newX + scaleX) - scale.x;
		intNewX = static_cast<int>(newX);
	}

	if (tileMap->IsCollisionTile(intNewX, intNewY)
		|| tileMap->IsCollisionTile(static_cast<int>(newX + scaleX), intNewY))
	{
		//上
		//this->isHitCollisionTile = true;
		rigidbody->velocity.y = -rigidbody->velocity.y * std::abs(1 - bounce);
		if (static_cast<int>(std::abs(rigidbody->velocity.y)) == 0)
		{
			rigidbody->velocity.y = 0;
		}
		//rigidbody->newVelocity.y = -rigidbody->newVelocity.y;
		newY = intNewY + 1.0f;
		intNewY = static_cast<int>(newY);

	}

	if (tileMap->IsCollisionTile(intNewX, static_cast<int>(newY + scaleY))
		|| tileMap->IsCollisionTile(static_cast<int>(newX + scaleX), static_cast<int>(newY + scaleY)))
	{
		//下
		//this->isHitCollisionTile = true;
		//isGrounded = true;
		rigidbody->velocity.y = -rigidbody->velocity.y * std::abs(1 - bounce);
		if (static_cast<int>(std::abs(rigidbody->velocity.y)) == 0)
		{
			rigidbody->velocity.y = 0;
		}
		//rigidbody->newVelocity.y = -rigidbody->newVelocity.y;
		newY = static_cast<int>(newY + scaleY) - scale.y;
		intNewY = static_cast<int>(newY);

	}

	//rigidbody->newPosition.x = newX;
	//rigidbody->newPosition.y = newY;
}
