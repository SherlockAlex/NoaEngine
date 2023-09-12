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

	extern void ApplyRigidbodysCollision();

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
		this->newPosition = colliderPos->position;
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
		
		//根据速度进行物体的碰撞检测
		//如果检测到了碰撞字符，就停止
		if (collision.isTrigger && collision.other != nullptr)
		{
			OnTrigger(collision.other);
		}
		collision.other = nullptr;

		//indexInMap = (int)(colliderPos->position.x) + (int)(colliderPos->position.y) * tileMap->w;
		const int x = colliderPos->position.x;
		const int y = colliderPos->position.y;
		indexInMap = (x << 16) | y;

		collision.other = nullptr;
		collision.isHitCollisionTile = false;

		if (useMotion&&useGravity&&(!collision.isGrounded)) 
		{
			velocity.y += 3.5 * g * deltaTime;
		}

		if (useMotion)
		{

			//处理力和速度的关系
			//F = ma
			velocity = (velocity * (1 - damping)) + (sumForce * (deltaTime * invMass));
			newPosition = (colliderPos->position) + (velocity * deltaTime);
			ApplyCollision();
			//处理碰撞检测
			colliderPos->position = newPosition;
		}
		

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

		if (velocity.x <= 0)
		{

			if (
				tileMap->IsCollisionTile(newPosition.x, colliderPos->position.y)
				|| tileMap->IsCollisionTile(newPosition.x, colliderPos->position.y + 0.999)
				)

			{

				if (!collision.isTrigger)
				{
					collision.isHitCollisionTile = true;
					newPosition.x = (int)newPosition.x + 1;
					velocity.x = 0;
				}

			}
		}
		else if (
				tileMap->IsCollisionTile(newPosition.x + 0.999, colliderPos->position.y)
				|| tileMap->IsCollisionTile(newPosition.x + 0.999, colliderPos->position.y + 0.999)
				)
		{
			if (!collision.isTrigger)
			{
				collision.isHitCollisionTile = true;
				newPosition.x = (int)newPosition.x;
				velocity.x = 0;
			}

		}

		collision.isGrounded = false;
		if (velocity.y <= 0)
		{
			if (
				tileMap->IsCollisionTile(newPosition.x, newPosition.y)
				|| tileMap->IsCollisionTile(newPosition.x + 0.999, newPosition.y)
				)
			{

				if (!collision.isTrigger)
				{
					collision.isHitCollisionTile = true;
					newPosition.y = (int)newPosition.y + 1;
					velocity.y = 0;
				}
			}
		}
		else if (
				tileMap->IsCollisionTile(newPosition.x, newPosition.y + 0.999)
				|| tileMap->IsCollisionTile(newPosition.x + 0.999f, newPosition.y + 0.999)
				)

		{

			if (!collision.isTrigger) {
				collision.isHitCollisionTile = true;
				newPosition.y = (int)newPosition.y;
				velocity.y = 0;
				collision.isGrounded = true;
			}

		}

		CollisionWithinRigidbody(this);

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

		//需要进行大优化，重复计算量大
		//对于两个刚体之间，只要进行一次碰撞检测便可
		//同时保存与之相碰撞的刚体到其信息中
		//然后rigid再去四位维数组[x1][y1][x2][y2]处查看保存的与之相撞的刚体是否为空
		//刚体之间的碰撞只要每一帧去执行一次便可

		if (rigid == nullptr||rigid->isFrozen)
		{
			return false;
		}

		if (rigid->collision.other!=nullptr)
		{
			if (rigid->collision.isTrigger)
			{
				return false;
			}
			return true;

		}

		const size_t hashCode = rigid->GetHashCode();

		for (const auto & e:rigidbodys)
		{
			if (e.first == hashCode || e.second == nullptr||e.second->collision.other!=nullptr)
			{
				continue;
			}

			Rigidbody* rigidbody = e.second;

			//检测两个刚体是否会相撞
			const float x1 = rigid->newPosition.x + 0.5;
			const float y1 = rigid->newPosition.y + 0.5;

			const float x2 = rigidbody->colliderPos->position.x + 0.5;
			const float y2 = rigidbody->colliderPos->position.y + 0.5;

			const float deltaX = x1 - x2;
			const float deltaY = y1 - y2;

			const float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);

			const float distanceValue = rigid->radius + rigidbody->radius;

			if (distance>=distanceValue)
			{
				continue;
			}

			rigid->collision.other = rigidbody;
			rigidbody->collision.other = rigid;
			if (rigid->collision.isTrigger 
				|| rigidbody->collision.isTrigger
				|| rigidbody->isFrozen
				)
			{
				return false;
			}

			//处理碰撞后的结果
			const float dr = distanceValue - distance;
			const float dx = dr * abs(deltaX) / distance+0.01;
			const float dy = dr * abs(deltaY) / distance+0.01;

			if (rigid->velocity.x > 0)
			{
				rigid->newPosition.x -= dx;
				rigid->velocity.x = 0;
			}
			else if(rigid->velocity.x<0)
			{
				rigid->newPosition.x += dx;
				rigid->velocity.x = 0;
			}

			if (rigid->velocity.y > 0)
			{
				rigid->newPosition.y -= dy;
				rigid->velocity.y = 0;
			}
			else if(rigid->velocity.y<0)
			{
				rigid->newPosition.y += dy;
				rigid->velocity.y = 0;
			}

			return true;

		}

		return false;

	}

	void ApplyRigidbodysCollision() 
	{
		//检测物体碰撞

		

	}

}

