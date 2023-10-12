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

		this->isRemoved = false;

		collision.other = nullptr;
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
		if (isRemoved)
		{
			return;
		}
		PhysicsSystem::rigidbodys.push_back(this);
	}

	void Rigidbody::UpdateVelocity(float deltaTime)
	{

		if (!GetActive())
		{
			return;
		}

		if (useMotion&&useGravity&&(!collision.isGrounded)) 
		{
			this->force += (gravity *gravityWeight);
		}

		if (useMotion)
		{
			velocity = (velocity * (1 - damping)) + (this->force * (deltaTime * invMass));
			newPosition = (this->actor->transform.position) + (velocity * deltaTime);
			
			ApplyCollision();
		}

		

		this->force = {};

	}

	void Rigidbody::UpdatePosition(float deltaTime)
	{
		if (!GetActive())
		{
			return;
		}
		ApplyCollision();
		

		if (collision.isHitCollisionTile && this->actor != nullptr)
		{
			this->actor->OnHitTile();
		}
		collision.isHitCollisionTile = false;

		if (useMotion && (!isFrozen)&&actor!=nullptr)
		{
			collision.isGrounded = false;
			collision.hitTileID = -1;
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
		if (this->tileMap==nullptr)
		{
			Debug::Error("Load tile map failed");
			exit(-1);
		}
	}

	Vector<float> pos(0.0, 0.0);
	void Rigidbody::ApplyCollision()
	{
		if (!GetActive() ||!useCollision||tileMap == nullptr)
		{
			return;
		}

		const float scaleX = collision.sacle.x;
		const float scaleY = collision.sacle.y;

		if (tileMap->IsCollisionTile(static_cast<int>(newPosition.x - scaleX), static_cast<int>(this->actor->transform.position.y - scaleY))
			|| tileMap->IsCollisionTile(static_cast<int>(newPosition.x - scaleX), static_cast<int>(this->actor->transform.position.y + 0.999 + scaleY))
		)
		{
			collision.isHitCollisionTile = true;
			if (!collision.isTrigger)
			{
				
				newPosition.x = (int)newPosition.x + 1 + scaleX;
				velocity.x = 0;
			}
		}

		if (tileMap->IsCollisionTile(static_cast<int>(newPosition.x + 0.999 + scaleX), static_cast<int>(this->actor->transform.position.y - scaleY))
			|| tileMap->IsCollisionTile(static_cast<int>(newPosition.x + 0.999 + scaleX), static_cast<int>(this->actor->transform.position.y + 0.999 + scaleY))
		)
		{
			collision.isHitCollisionTile = true;
			if (!collision.isTrigger)
			{
				
				newPosition.x = (int)newPosition.x - scaleX;
				velocity.x = 0;
			}

		}

		if (tileMap->IsCollisionTile(static_cast<int>(newPosition.x - scaleX), static_cast<int>(newPosition.y - scaleY))
			|| tileMap->IsCollisionTile(static_cast<int>(newPosition.x + 0.999 + scaleX), static_cast<int>(newPosition.y - scaleY))
		)
		{
			collision.isHitCollisionTile = true;
			if (!collision.isTrigger)
			{
				
				newPosition.y = (int)newPosition.y + 1 + scaleY;
				velocity.y = 0;
			}
		}

		if (tileMap->IsCollisionTile(static_cast<int>(newPosition.x - scaleX), static_cast<int>(newPosition.y + 0.999 + scaleY))
			|| tileMap->IsCollisionTile(static_cast<int>(newPosition.x + 0.999 + scaleX), static_cast<int>(newPosition.y + 0.999 + scaleY))
		)
		{
			collision.isHitCollisionTile = true;
			if (!collision.isTrigger) {
				collision.isGrounded = true;
				
				newPosition.y = (int)newPosition.y - scaleY;
				velocity.y = 0;
			}
		}

	}

	void Rigidbody::BindCollider(Collider2D* collider) 
	{
		colliders.push_back(collider);
	}

	void Rigidbody::ApplyTrigger() {
		if (GetActive() &&collision.isTrigger && collision.other != nullptr && this->actor != nullptr)
		{
			this->actor->OnTrigger(collision);
		}
		collision.other = nullptr;
	}

	TileMap* Rigidbody::GetTileMap()
	{
		return this->tileMap;
	}

	Rigidbody* Rigidbody::Create(Actor* actor)
	{
		return new Rigidbody(actor);
	}

	void Rigidbody::Destroy()
	{
		this->isRemoved = true;
	}

}

