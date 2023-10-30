#include "Actor.h"
#include "ActorComponent.h"
#include "Scene.h"
#include "Debug.h"
#include "Physics.h"
#include "PhysicsSystem.h"
#include "Collider2D.h"

#include <unordered_map>
#include <thread>
#include <stack>

using namespace std;

noa::Rigidbody::Rigidbody(Actor* actor) :ActorComponent(actor)
{

	collision.actor = nullptr;
	this->velocity = { 0,0 };
	invMass = 1.0f / mass;
	colliders.reserve(10);
	if (actor&& actor->GetActiveScene()&& actor->GetActiveScene()->GetTileMap())
	{
		this->SetTileMap(actor->GetActiveScene()->GetTileMap()->GetLevelAs<TileMap>());
	}

}

noa::Rigidbody::~Rigidbody()
{
	if (!colliders.empty())
	{
		for (auto& collider : colliders)
		{
			collider->rigidbody = nullptr;
		}
	}

}

void noa::Rigidbody::Start()
{

}

void noa::Rigidbody::Update()
{
	PhysicsSystem::rigidbodys.push_back(this);
}

void noa::Rigidbody::UpdateVelocity(float deltaTime)
{

	if (!GetActive())
	{
		return;
	}

	if (useMotion && useGravity && (!isGrounded))
	{
		this->force += (PhysicsSystem::gravity * gravityWeight);
	}

	if (useMotion)
	{
		velocity = (velocity * (1 - damping)) + (this->force * (deltaTime * invMass));
		newPosition = (GetActor()->transform.position) + (velocity * deltaTime);

	}

	this->ApplyTileCollision();

	this->force = {};

}

void noa::Rigidbody::UpdatePosition(float deltaTime)
{
	if (!GetActive())
	{
		return;
	}

	this->ApplyTileCollision();



	if (useMotion && (!isFrozen) && GetActor() != nullptr)
	{
		isGrounded = false;
		GetActor()->transform.position = newPosition;
	}
}

void noa::Rigidbody::AddForce(const Vector<float>& force, ForceType forceType)
{
	//添加力到物体上

	switch (forceType)
	{
	case ForceType::CONTINUOUS_FORCE:
		//添加恒力到物体上
		this->force = this->force + force;
		break;
	case ForceType::IMPULSE_FORCE:
		//添加一个冲量到物体上，作用完马上就消失
		velocity += force * invMass;
		break;
	default:
		break;
	}

}

void noa::Rigidbody::SetTileMap(TileMap* map)
{
	this->tileMap = map;
	for (auto& collider : colliders)
	{
		if (collider)
		{
			collider->SetTileMap(map);
		}
	}
	if (this->tileMap == nullptr)
	{
		Debug::Error("Load tile map failed");
		exit(-1);
	}
}

void noa::Rigidbody::SetMass(float value)
{
	this->mass = value;
	this->invMass = 1.0f / value;
}

void noa::Rigidbody::SetTileColliderScale(float x, float y)
{
	this->tileColliderSacle.x = x;
	this->tileColliderSacle.y = y;
}

noa::Vector<float> pos(0.0, 0.0);
void noa::Rigidbody::ApplyTileCollision()
{
	if (!GetActive() || !useCollision || tileMap == nullptr)
	{
		return;
	}

	if (tileColliderSacle.x == 0 || tileColliderSacle.y == 0)
	{
		return;
	}

	float newX = newPosition.x;
	float newY = newPosition.y;
	float posX = GetActor()->transform.position.x;
	float posY = GetActor()->transform.position.y;

	int intNewX = static_cast<int>(newX);
	int intNewY = static_cast<int>(newY);
	int intPosX = static_cast<int>(posX);
	int intPosY = static_cast<int>(posY);

	float deltaStep = 0.999f;
	float scaleX = deltaStep *std::abs(this->tileColliderSacle.x);
	float scaleY = deltaStep *std::abs(this->tileColliderSacle.y);

	if (tileMap->IsCollisionTile(intNewX,intPosY)
		||tileMap->IsCollisionTile(intNewX,posY + scaleY)
		) 
	{
		//左
		for (auto & collider:colliders)
		{
			if (collider)
			{
				collider->isHitCollisionTile = true;
			}
		}

		velocity.x = 0;
		newX = intNewX + 1;
		intNewX = static_cast<int>(newX);
	}

	if (tileMap->IsCollisionTile(newX+ scaleX,intPosY)
		||tileMap->IsCollisionTile(newX+ scaleX,posY+ scaleY)
		) 
	{
		//右
		for (auto& collider : colliders)
		{
			if (collider)
			{
				collider->isHitCollisionTile = true;
			}
		}
		velocity.x = 0;
		newX = static_cast<int>(newX+scaleX) - tileColliderSacle.x;
		intNewX = static_cast<int>(newX);
	}

	if (tileMap->IsCollisionTile(intNewX,intNewY)
		||tileMap->IsCollisionTile(newX+ scaleX,intNewY))
	{
		//上
		for (auto& collider : colliders)
		{
			if (collider)
			{
				collider->isHitCollisionTile = true;
			}
		}

		velocity.y = 0;
		newY = intNewY + 1;
		intNewY = static_cast<int>(newY);

	}

	if (tileMap->IsCollisionTile(intNewX, newY + scaleY)
		|| tileMap->IsCollisionTile(newX + scaleX, newY + scaleY))
	{
		//下
		for (auto& collider : colliders)
		{
			if (collider)
			{
				collider->isHitCollisionTile = true;
			}
		}

		isGrounded = true;
		velocity.y = 0;
		newY = static_cast<int>(newY + scaleY) - tileColliderSacle.y;
		intNewY = static_cast<int>(newY);

	}

	newPosition.x = newX;
	newPosition.y = newY;

}

void noa::Rigidbody::BindCollider(Collider2D* collider)
{
	collider->SetTileMap(this->tileMap);
	colliders.push_back(collider);
}

noa::TileMap* noa::Rigidbody::GetTileMap()
{
	return this->tileMap;
}

noa::Rigidbody* noa::Rigidbody::Create(Actor* actor)
{
	return noa::NObject<Rigidbody>::Create(actor);
}

bool noa::Collision::CompareTag(const std::string& tag) const
{
	if (actor == nullptr)
	{
		return false;
	}
	return actor->tag == tag;
}


