#include "Physics.h"
#include "NoaEngine.h"
#include "Scene.h"
#include <unordered_map>
#include <thread>

using namespace std;

namespace noa {

	extern float deltaTime;
	//检测rigid是否和其他刚体相撞，如果相撞就返回
	extern bool CollisionWithinRigidbody(Rigidbody* rigid, const int x1, const int y1,const int x2,const int y2);

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
				CollisionWithinRigidbody(this
					, newPosition.x, colliderPos->position.y
					, newPosition.x, colliderPos->position.y + 0.999
				)
				|| tileMap->IsCollisionTile(newPosition.x, colliderPos->position.y)
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
				CollisionWithinRigidbody(this
					, newPosition.x + 0.999, colliderPos->position.y
					, newPosition.x + 0.999, colliderPos->position.y + 0.999
				)
				|| tileMap->IsCollisionTile(newPosition.x + 0.999, colliderPos->position.y)
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
				CollisionWithinRigidbody(this,
					newPosition.x, newPosition.y
					, newPosition.x + 0.999, newPosition.y
				)
				|| tileMap->IsCollisionTile(newPosition.x, newPosition.y)
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
				CollisionWithinRigidbody(this
					, newPosition.x, newPosition.y + 0.999
					, newPosition.x + 0.999, newPosition.y + 0.999
				)
				|| tileMap->IsCollisionTile(newPosition.x, newPosition.y + 0.999)
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

		//if (velocity.x <= 0)
		//{
		//	//向下
		//	if (
		//		CollisionWithinRigidbody(this
		//			, newPosition.x - colliderSize.x - 0.4999, colliderPos->position.y - colliderSize.y - 0.4999
		//			, newPosition.x - colliderSize.x - 0.4999, colliderPos->position.y + 0.4999 + colliderSize.y
		//		)
		//		|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x - colliderSize.x - 0.4999, colliderPos->position.y - colliderSize.y - 0.4999))
		//		|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x - colliderSize.x - 0.4999, colliderPos->position.y + colliderSize.y + 0.4999))
		//		)

		//	{

		//		if (!collision.isTrigger)
		//		{
		//			collision.isHitCollisionTile = true;
		//			//newPosition.x = (int)newPosition.x + 1 + colliderSize.x + 0.5;
		//			newPosition.x = (int)newPosition.x + colliderSize.x + 0.5;
		//			velocity.x = 0;
		//		}

		//	}
		//}
		//else
		//{
		//	if (
		//		CollisionWithinRigidbody(this
		//			, newPosition.x + 0.4999 + colliderSize.x, colliderPos->position.y - colliderSize.y - 0.4999
		//			, newPosition.x + 0.4999 + colliderSize.x, colliderPos->position.y + 0.4999 + colliderSize.y
		//		)
		//		|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.4999 + colliderSize.x, colliderPos->position.y - colliderSize.y- 0.4999))
		//		|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.4999 + colliderSize.x, colliderPos->position.y + 0.4999 + colliderSize.y))
		//		)
		//	{
		//		if (!collision.isTrigger)
		//		{
		//			collision.isHitCollisionTile = true;
		//			newPosition.x = (int)newPosition.x + 0.5 - colliderSize.x;
		//			velocity.x = 0;
		//		}

		//	}
		//}

		//collision.isGrounded = false;
		//if (velocity.y <= 0)
		//{
		//	if (
		//		CollisionWithinRigidbody(this,
		//			newPosition.x - colliderSize.x - 0.4999, newPosition.y - colliderSize.y - 0.4999
		//			, newPosition.x + 0.4999 + colliderSize.x, newPosition.y - colliderSize.y - 0.4999
		//		)
		//		|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x - colliderSize.x - 0.4999, newPosition.y - colliderSize.y - 0.4999))
		//		|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.4999 + colliderSize.x, newPosition.y - colliderSize.y - 0.4999))
		//		)
		//	{

		//		if (!collision.isTrigger)
		//		{
		//			collision.isHitCollisionTile = true;
		//			//newPosition.y = (int)newPosition.y + 1 + colliderSize.y + 0.5;
		//			newPosition.y = (int)newPosition.y + colliderSize.y + 0.5;
		//			velocity.y = 0;
		//		}
		//	}
		//}
		//else
		//{
		//	if (
		//		CollisionWithinRigidbody(this
		//			, newPosition.x - colliderSize.x - 0.4999, newPosition.y + 0.4999 + colliderSize.y
		//			, newPosition.x + 0.4999 + colliderSize.x, newPosition.y + 0.4999 + colliderSize.y
		//		)
		//		|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x - colliderSize.x - 0.4999, newPosition.y + 0.4999 + colliderSize.y))
		//		|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.4999 + colliderSize.x, newPosition.y + 0.4999 + colliderSize.y))
		//		)

		//	{

		//		if (!collision.isTrigger) {
		//			collision.isHitCollisionTile = true;
		//			newPosition.y = (int)newPosition.y - colliderSize.y + 0.5;
		//			velocity.y = 0;
		//			collision.isGrounded = true;
		//		}

		//	}
		//}

	}

	int Rigidbody::GetIndexInMap() const
	{
		return this->indexInMap;
	}

	void Rigidbody::RemoveRigidbody() const
	{
		DestroyRigidbody(this);
	}

	


	bool CollisionWithinRigidbody(Rigidbody* rigid, const int x1, const int y1, const int x2, const int y2)
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

		//indexInMap是一个动态的概念，获取唯一的map值
		const int indexInMap1 = (x1<<16)|y1;
		const int indexInMap2 = (x2<<16)|y2;

		const size_t hashCode = rigid->GetHashCode();

		for (const auto & pair:rigidbodys) 
		{
			if (pair.first == hashCode||pair.second == nullptr)
			{
				continue;
			}
			Rigidbody* rigidbody = pair.second;
			const int indexOfRigid = rigidbody->GetIndexInMap();
			if (indexOfRigid!=indexInMap1&&indexOfRigid!=indexInMap2)
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
			return true;

		}
		return false;

	}

	void ApplyRigidbodysCollision() 
	{
		//检测物体碰撞

		//检测所有物体的[0][0]:上
		//检测所有物体的[0][1]:下
		//检测所有物体的[1][0]:左
		//检测所有物体的[1][1]:右

		
		

	}

}

