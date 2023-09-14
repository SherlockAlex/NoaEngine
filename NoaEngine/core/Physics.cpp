#include "Physics.h"
#include "NoaEngine.h"
#include "Scene.h"
#include <unordered_map>
#include <thread>

using namespace std;

namespace noa {

	extern float deltaTime;
	//检测rigid是否和其他刚体相撞，如果相撞就返回
	extern bool CollisionWithinRigidbody(Rigidbody* rigid);
	extern bool BoundingBoxIntersect(Rigidbody* a, Rigidbody* b);
	float gravityAcceration = 9.81;

	unordered_map<size_t,Rigidbody*> rigidbodys;
	unordered_map<size_t, bool> isCheckCollision;

	void DestroyRigidbody(const Rigidbody * rigid)
	{
		rigidbodys[rigid->GetHashCode()] = nullptr;
		Debug("rigidbody has been done");
	}

	Rigidbody::Rigidbody(Transform* colliderPos)
	{

		id = GetNextId();
		collision.other = nullptr;
		this->colliderPos = colliderPos;
		this->velocity = { 0,0 };
		invMass = 1.0 / mass;

		rigidbodys[GetHashCode()] = this;
	}

	// 初始化静态计数器
	size_t Rigidbody::nextId = 0;

	Rigidbody::~Rigidbody()
	{
		DestroyRigidbody(this);
	}

	
	void Rigidbody::Start()
	{

	}

	//实现物理效果
	void Rigidbody::Update()
	{
		if (isFrozen)
		{
			return;
		}
		
		const int x = colliderPos->position.x;
		const int y = colliderPos->position.y;
		indexInMap = (x << 16) | y;

		collision.other = nullptr;
		collision.isHitCollisionTile = false;

		if (deltaTime>1.0/10)
		{
			deltaTime = 1.0 / 60;
		}

		if (useMotion&&useGravity&&(!collision.isGrounded)) 
		{
			velocity.y += gravityWeight * gravityAcceration * deltaTime;
		}

		if (useMotion)
		{

			//处理力和速度的关系
			//F = ma
			velocity = (velocity * (1 - damping)) + (sumForce * (deltaTime * invMass));
			newPosition = (colliderPos->position) + (velocity * deltaTime);
			collision.isGrounded = false;
			ApplyCollision();
			//处理碰撞检测
			CollisionWithinRigidbody(this);
			ApplyCollision();
			ApplyCollision();
			colliderPos->position = newPosition;
		}
		
		//根据速度进行物体的碰撞检测
		//如果检测到了碰撞字符，就停止
		if (collision.isTrigger && collision.other != nullptr)
		{
			OnTrigger(collision.other);
		}
		collision.other = nullptr;

	}

	void Rigidbody::AddForce(const Vector<float> force, ForceType forceType)
	{
		//添加力到物体上
		switch (forceType)
		{
		case Rigidbody::ContinuousForce:
			//添加恒力到物体上
			sumForce += force;
			break;
		case Rigidbody::Impulse:
			//添加一个冲量到物体上，作用完马上就消失
			velocity += force * invMass;
			break;
		default:
			break;
		}

	}

	void Rigidbody::UpdateMap(TileMap* map)
	{
		this->tileMap = map;
		if (this->tileMap==nullptr)
		{
			Debug("Load tile map failed");
			exit(-1);
		}
	}

	Vector<float> pos(0.0, 0.0);
	void Rigidbody::ApplyCollision()
	{
		if (!useCollision||tileMap == nullptr)
		{
			return;
		}

		const float scaleX = collision.sacle.x;
		const float scaleY = collision.sacle.y;

		if (tileMap->IsCollisionTile(newPosition.x-scaleX, colliderPos->position.y - scaleY)
			|| tileMap->IsCollisionTile(newPosition.x-scaleX, colliderPos->position.y + 0.999 + scaleY)
		)
		{
			if (!collision.isTrigger)
			{
				collision.isHitCollisionTile = true;
				newPosition.x = (int)newPosition.x + 1 + scaleX;
				velocity.x = 0;
			}
		}

		if (tileMap->IsCollisionTile(newPosition.x + 0.999 + scaleX, colliderPos->position.y - scaleY)
			|| tileMap->IsCollisionTile(newPosition.x + 0.999 + scaleX, colliderPos->position.y + 0.999 + scaleY)
		)
		{
			if (!collision.isTrigger)
			{
				collision.isHitCollisionTile = true;
				newPosition.x = (int)newPosition.x - scaleX;
				velocity.x = 0;
			}

		}

		if (tileMap->IsCollisionTile(newPosition.x - scaleX, newPosition.y - scaleY)
			|| tileMap->IsCollisionTile(newPosition.x + 0.999 + scaleX, newPosition.y - scaleY)
		)
		{
			if (!collision.isTrigger)
			{
				collision.isHitCollisionTile = true;
				newPosition.y = (int)newPosition.y + 1 + scaleY;
				velocity.y = 0;
			}
		}

		if (tileMap->IsCollisionTile(newPosition.x - scaleX, newPosition.y + 0.999 + scaleY)
			|| tileMap->IsCollisionTile(newPosition.x + 0.999f+scaleX, newPosition.y + 0.999 + scaleY)
		)
		{
			if (!collision.isTrigger) {
				collision.isGrounded = true;
				collision.isHitCollisionTile = true;
				newPosition.y = (int)newPosition.y - scaleY;
				velocity.y = 0;
			}
		}

	}

	int Rigidbody::GetIndexInMap() const
	{
		return this->indexInMap;
	}

	void Rigidbody::RemoveRigidbody() const
	{
		DestroyRigidbody(this);
	}

	bool CollisionWithinRigidbody(Rigidbody* rigid)
	{
		if (rigid == nullptr || rigid->isFrozen)
		{
			return false;
		}

		const size_t hashCode = rigid->GetHashCode();
		const float rigidRadius = rigid->collision.radius;
		const float rigidX = rigid->newPosition.x;
		const float rigidY = rigid->newPosition.y;
		Vector<float>& rigidVelocity = rigid->velocity;

		bool collisionOccurred = false;

		for (const auto& e : rigidbodys)
		{
			if (e.first == hashCode || e.second == nullptr)
			{
				continue;
			}

			Rigidbody* rigidbody = e.second;

			// 检查包围盒是否相交
			if (!BoundingBoxIntersect(rigid, rigidbody))
			{
				continue;
			}

			// 检测两个刚体是否会相撞（使用平方距离）
			const float deltaX = rigidX - rigidbody->colliderPos->position.x;
			const float deltaY = rigidY - rigidbody->colliderPos->position.y;
			const float distanceSquared = deltaX * deltaX + deltaY * deltaY;
			const float radiusSumSquared = (rigidRadius + rigidbody->collision.radius) * (rigidRadius + rigidbody->collision.radius);

			if (distanceSquared >= radiusSumSquared)
			{
				continue;
			}

			rigid->collision.other = rigidbody;

			if (rigid->collision.isTrigger || rigidbody->collision.isTrigger || rigidbody->isFrozen)
			{
				return false;
			}

			// 处理碰撞后的结果
			const float distance = sqrt(distanceSquared);
			const float dr = sqrt(radiusSumSquared) - distance;
			const float dx = dr * deltaX / distance;
			const float dy = dr * deltaY / distance;

			rigid->newPosition.x += dx;
			rigid->newPosition.y += dy;
			
			rigid->ApplyCollision();

			rigidVelocity.x = 0;
			rigidVelocity.y = 0;

			collisionOccurred = true;
		}

		return collisionOccurred;
	}

	bool BoundingBoxIntersect(Rigidbody* a, Rigidbody* b)
	{
		// 实现包围盒相交检测逻辑，如果相交返回true，否则返回false
		// 你可以根据刚体的形状和位置来实现具体的检测方法
		// 这里简单示范一个矩形包围盒的检测
		return (a->newPosition.x - a->collision.radius < b->newPosition.x + b->collision.radius) &&
			(a->newPosition.x + a->collision.radius > b->newPosition.x - b->collision.radius) &&
			(a->newPosition.y - a->collision.radius < b->newPosition.y + b->collision.radius) &&
			(a->newPosition.y + a->collision.radius > b->newPosition.y - b->collision.radius);
	}

}

