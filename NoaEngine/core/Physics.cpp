#include "Physics.h"
#include "NoaEngine.h"
#include "Scene.h"
#include <unordered_map>

using namespace std;

namespace noa {

	extern float deltaTime;

	unordered_map<int,bool> collisionTiles;

	extern bool IsCollisionTile(int tileID);
	//检测rigid是否和其他刚体相撞，如果相撞就返回
	//extern bool CollisionWithinRigidbody(Rigidbody* rigid,const int x,const int y);
	extern bool CollisionWithinRigidbody(Rigidbody* rigid, const int x1, const int y1,const int x2,const int y2);

	//void ApplyCollision(Rigidbody* rigid, int posx, int posy);

	unordered_map<size_t,Rigidbody*> rigidbodys;

	TileMap* tileMap = nullptr;

	Vector<float> sumForce(0.0, 0.0);

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
		indexInMap = (int)(colliderPos->position.x) + (int)(colliderPos->position.y) * tileMap->w;
		collision.other = nullptr;
		collision.isHitCollisionTile = false;
		if (!isFrozen)
		{
			if (useGravity)
			{
				if (!collision.isGrounded)
				{
					//如果使用重力
					velocity.y += 3.5 * g * deltaTime;

				}
			}

			//处理力和速度的关系
			//F = ma

			velocity = (velocity * (1 - damping)) + (sumForce * (deltaTime * invMass));
		}
		

		//将速度的量反馈到物体的位移变化
		Vector<float> newPosition= move((colliderPos->position) + (velocity * deltaTime));
		
		//根据速度进行物体的碰撞检测
		//如果检测到了碰撞字符，就停止

		if (useCollision)
		{
			if (velocity.x <= 0)
			{

				if (ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x, colliderPos->position.y + 0.0))
					||ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x, colliderPos->position.y + 0.999))
					||CollisionWithinRigidbody(this
						,newPosition.x,colliderPos->position.y
						, newPosition.x, colliderPos->position.y + 0.999
					)
					//||CollisionWithinRigidbody(this, newPosition.x, colliderPos->position.y+0.999)
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
			else
			{
				if (ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.999, colliderPos->position.y + 0.0))
					||ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.999, colliderPos->position.y + 0.999))
					|| CollisionWithinRigidbody(this
						, newPosition.x+0.999, colliderPos->position.y
						, newPosition.x + 0.999, colliderPos->position.y + 0.999
					)
					//|| CollisionWithinRigidbody(this, newPosition.x+0.999, colliderPos->position.y + 0.999)
					)
				{
					if (!collision.isTrigger)
					{
						collision.isHitCollisionTile = true;
						newPosition.x = (int)newPosition.x;
						velocity.x = 0;
					}
					
				}
			}

			collision.isGrounded = false;
			if (velocity.y <= 0)
			{
				if (ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.0f, newPosition.y))
					||ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.999f, newPosition.y))
					|| CollisionWithinRigidbody(this,
						newPosition.x, newPosition.y
						, newPosition.x + 0.999, newPosition.y
					)
					//|| CollisionWithinRigidbody(this, newPosition.x+0.999, newPosition.y)
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
			else
			{
				if (ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.0f, newPosition.y + 0.999))
					||ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.999f, newPosition.y + 0.999))
					|| CollisionWithinRigidbody(this
						, newPosition.x, newPosition.y+0.999
						, newPosition.x + 0.999, newPosition.y + 0.999
					)
					//|| CollisionWithinRigidbody(this, newPosition.x + 0.999, newPosition.y+0.999)
					)
				{

					if (!collision.isTrigger) {
						collision.isHitCollisionTile = true;
						newPosition.y = (int)newPosition.y;
						velocity.y = 0;
						collision.isGrounded = true;
					}
					
				}
			}

			

		}

		if (collision.isTrigger)
		{
			OnTrigger(collision.other);
		}
		collision.other = nullptr;
		

		//ApplyCollision(this, colliderPos->position.x, colliderPos->position.y);

		/*if (velocity.x<=0)
		{
			ApplyCollision(this, newPosition.x, colliderPos->position.y);
			ApplyCollision(this, newPosition.x, colliderPos->position.y);
		}
		else {
			ApplyCollision(this, newPosition.x + 0.999, colliderPos->position.y);
			ApplyCollision(this, newPosition.x + 0.999, colliderPos->position.y + 0.999);
		}
		
		if (velocity.y<=0)
		{
			ApplyCollision(this, newPosition.x, newPosition.y);
			ApplyCollision(this, newPosition.x + 0.999, newPosition.y);
		}
		else {
			ApplyCollision(this, newPosition.x, newPosition.y + 0.999);
			ApplyCollision(this, newPosition.x + 0.999, newPosition.y + 0.999);
		}*/

		colliderPos->position = move(newPosition);
		//Debug("isGrounded:" + to_string(isGrounded));

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

	void Rigidbody::SetCollisionTileID(std::vector<int> collisionTileIDs)
	{
		//设置Collision Tiles
		const int collisionTilesCount = collisionTileIDs.size();
		for (int i=0;i< collisionTilesCount;i++)
		{
			collisionTiles[collisionTileIDs[i]] = true;
		}
	}

	void Rigidbody::UpdateMap(void* map)
	{
		tileMap = static_cast<TileMap*>(map);
	}

	Vector<float> pos(0.0, 0.0);

	float Rigidbody::FixPosition()
	{
		//修复玩家位置
		return 0.0;
	}

	void Rigidbody::UpdateCollision(const Vector<float> nextPosition)
	{
		
	}

	int Rigidbody::GetIndexInMap() const
	{
		return this->indexInMap;
	}

	void Rigidbody::RemoveRigidbody() const
	{
		DestroyRigidbody(this);
	}

	bool IsCollisionTile(int tileID) 
	{
		return ContainKey<int, bool>(collisionTiles, tileID);
	}

	//bool CollisionWithinRigidbody(Rigidbody * rigid,const int x,const int y)
	//{

	//	//indexInMap是一个动态的概念
	//	const int indexInMap = y * tileMap->w + x;

	//	for (int i=0;i< rigidbodys.size();i++)
	//	{
	//		if (rigidbodys[i] == nullptr||rigid == nullptr||rigidbodys[i]==rigid)
	//		{
	//			continue;
	//		}
	//		if (rigidbodys[i]->GetIndexInMap() == indexInMap)
	//		{
	//			return true;
	//		}
	//	}
	//	return false;
	//}

	bool CollisionWithinRigidbody(Rigidbody* rigid, const int x1, const int y1, const int x2, const int y2)
	{

		if (rigid == nullptr)
		{
			return false;
		}

		//indexInMap是一个动态的概念
		const int indexInMap1 = y1 * tileMap->w + x1;
		const int indexInMap2 = y2 * tileMap->w + x2;

		bool resultValue = false;

		const size_t hashCode = rigid->GetHashCode();

		for (auto & pair:rigidbodys) 
		{
			if (pair.first == hashCode||pair.second == nullptr)
			{
				continue;
			}
			Rigidbody* rigidbody = pair.second;
			//rigid->collision.other = nullptr;

			const int indexOfRigid = rigidbody->GetIndexInMap();

			if (indexOfRigid == indexInMap1||indexOfRigid == indexInMap2)
			{
				rigid->collision.other = rigidbody;
				resultValue = true;
				if (rigid->collision.isTrigger || rigidbody->collision.isTrigger)
				{
					resultValue = false;
				}
				return resultValue;
			}

		}

		resultValue = false;
		return resultValue;
	}

	//void ApplyCollision(Rigidbody* rigid,int posx,int posy)
	//{
	//	//indexInMap是一个动态的概念
	//	const int indexInMap = posy * tileMap->w + posx;

	//	for (int i = 0; i < rigidbodys.size(); i++)
	//	{
	//		if (rigidbodys[i] == nullptr||rigid == nullptr||rigidbodys[i] == rigid)
	//		{
	//			continue;
	//		}
	//		const int indexOfRigid = rigidbodys[i]->GetIndexInMap();
	//		if (indexOfRigid == indexInMap)
	//		{
	//			const float m1 = rigid->mass;
	//			const float m2 = rigidbodys[i]->mass;
	//			const Vector<float> v1 = rigid->velocity;
	//			const Vector<float> v2 = rigidbodys[i]->velocity;
	//			rigid->velocity = ((v1 * m1 + v2 * m2) / (m1 + m2)) * 2.0 - v1;
	//			rigidbodys[i]->velocity = ((v1 * m1 + v2 * m2) / (m1 + m2)) * 2.0 - v2;
	//			Debug("物体碰撞");
	//		}
	//	}
	//}

}

