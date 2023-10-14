#include "NoaEngine.h"

#include <unordered_map>
#include <thread>
#include <stack>

using namespace std;

namespace noa 
{
	float gravityAcceration = 9.81f;

	Vector<float> gravity = { 0,gravityAcceration };
	
	unordered_map<size_t, bool> isCheckCollision;

	Rigidbody::Rigidbody(Actor * actor) :ActorComponent(actor)
	{

		collision.actor = nullptr;
		this->actor = actor;
		this->velocity = { 0,0 };
		invMass = 1.0f / mass;
		colliders.reserve(10);

	}

	Rigidbody::~Rigidbody()
	{
		if (!colliders.empty()) 
		{
			for (auto & collider:colliders) 
			{
				collider->rigidbody = nullptr;
			}
		}

	}

	void Rigidbody::Start()
	{

	}

	void Rigidbody::Update()
	{
		PhysicsSystem::rigidbodys.push_back(this);
	}

	void Rigidbody::UpdateVelocity(float deltaTime)
	{

		if (!GetActive())
		{
			return;
		}

		if (useMotion&&useGravity&&(!isGrounded)) 
		{
			this->force += (gravity *gravityWeight);
		}

		if (useMotion)
		{
			velocity = (velocity * (1 - damping)) + (this->force * (deltaTime * invMass));
			newPosition = (this->actor->transform.position) + (velocity * deltaTime);

			this->ApplyTileCollision();

		}

		

		this->force = {};

	}

	void Rigidbody::UpdatePosition(float deltaTime)
	{
		if (!GetActive())
		{
			return;
		}
		
		this->ApplyTileCollision();
		
		

		if (useMotion && (!isFrozen)&&actor!=nullptr)
		{
			isGrounded = false;
			actor->transform.position = newPosition;
		}
	}

	void Rigidbody::AddForce(const Vector<float> & force, ForceType forceType)
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

	void Rigidbody::SetTileMap(TileMap* map)
	{
		this->tileMap = map;
		for (auto & collider:colliders) 
		{
			if (collider)
			{
				collider->SetTileMap(map);
			}
		}
		if (this->tileMap==nullptr)
		{
			Debug::Error("Load tile map failed");
			exit(-1);
		}
	}

	void Rigidbody::SetMass(float value)
	{
		this->mass = value;
		this->invMass = 1.0f / value;
	}

	void Rigidbody::SetTileColliderScale(float x, float y)
	{
		this->tileColliderSacle.x = x;
		this->tileColliderSacle.y = y;
	}

	Vector<float> pos(0.0, 0.0);
	void Rigidbody::ApplyTileCollision()
	{
		if (!GetActive() ||!useCollision||tileMap == nullptr)
		{
			return;
		}

		if (tileColliderSacle.x == 0||tileColliderSacle.y == 0) 
		{
			return;
		}

		const float scaleX = tileColliderSacle.x-1;
		const float scaleY = tileColliderSacle.y-1;

		if (tileMap->IsCollisionTile(static_cast<int>(newPosition.x - scaleX), static_cast<int>(this->actor->transform.position.y - scaleY))
			|| tileMap->IsCollisionTile(static_cast<int>(newPosition.x - scaleX), static_cast<int>(this->actor->transform.position.y + 0.999 + scaleY))
		)
		{
			for (auto & collider:colliders)
			{
				if (collider)
				{
					collider->isHitCollisionTile = true;
				}
			}
			newPosition.x = (int)newPosition.x + 1 + scaleX;
			velocity.x = 0;
		}

		if (tileMap->IsCollisionTile(static_cast<int>(newPosition.x + 0.999 + scaleX), static_cast<int>(this->actor->transform.position.y - scaleY))
			|| tileMap->IsCollisionTile(static_cast<int>(newPosition.x + 0.999 + scaleX), static_cast<int>(this->actor->transform.position.y + 0.999 + scaleY))
		)
		{
			for (auto& collider : colliders)
			{
				if (collider)
				{
					collider->isHitCollisionTile = true;
				}
			}
			newPosition.x = (int)newPosition.x - scaleX;
			velocity.x = 0;

		}

		if (tileMap->IsCollisionTile(static_cast<int>(newPosition.x - scaleX), static_cast<int>(newPosition.y - scaleY))
			|| tileMap->IsCollisionTile(static_cast<int>(newPosition.x + 0.999 + scaleX), static_cast<int>(newPosition.y - scaleY))
		)
		{
			for (auto& collider : colliders)
			{
				if (collider)
				{
					collider->isHitCollisionTile = true;
				}
			}
			newPosition.y = (int)newPosition.y + 1 + scaleY;
			velocity.y = 0;
		}

		if (tileMap->IsCollisionTile(static_cast<int>(newPosition.x - scaleX), static_cast<int>(newPosition.y + 0.999 + scaleY))
			|| tileMap->IsCollisionTile(static_cast<int>(newPosition.x + 0.999 + scaleX), static_cast<int>(newPosition.y + 0.999 + scaleY))
		)
		{
			for (auto& collider : colliders)
			{
				if (collider)
				{
					collider->isHitCollisionTile = true;
				}
			}
			isGrounded = true;

			newPosition.y = (int)newPosition.y - scaleY;
			velocity.y = 0;
		}

	}

	void Rigidbody::BindCollider(Collider2D* collider) 
	{
		collider->SetTileMap(this->tileMap);
		colliders.push_back(collider);
	}

	TileMap* Rigidbody::GetTileMap()
	{
		return this->tileMap;
	}

	Rigidbody* Rigidbody::Create(Actor* actor)
	{
		return new Rigidbody(actor);
	}

	bool Collision::CompareTag(const std::string& tag) const
	{
		if (actor == nullptr)
		{
			return false;
		}
		return actor->tag == tag;
	}

}

