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
	extern bool CollisionWithinRigidbody(Rigidbody* rigid,const int x,const int y);
	extern bool CollisionWithinRigidbody(Rigidbody* rigid, const int x1, const int y1,const int x2,const int y2);

	void ApplyCollision(Rigidbody* rigid, int posx, int posy);

	vector<Rigidbody*> rigidbodys;

	TileMap* tileMap = nullptr;

	Vector<float> sumForce(0.0, 0.0);

	void DestroyRigidbody(Rigidbody * rigid)
	{
		for (int i = 0;i<rigidbodys.size();i++) 
		{
			if (rigidbodys[i] == rigid)
			{
				rigidbodys[i] = nullptr;
				Debug("rigidbody has been done");
			}
		}
		//rigidbodys.erase(std::remove(rigidbodys.begin(), rigidbodys.end(), rigid), rigidbodys.end());
		
	}

	Rigidbody::Rigidbody(Transform* colliderPos)
	{
		collisionRigid = nullptr;
		this->colliderPos = colliderPos;
		invMass = 1.0 / mass;
		
		rigidbodys.push_back(this);
	}

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
		isHitWall = false;
		if (isFrozen)
		{
			return;
		}
		if (useGravity)
		{
			if (!isGrounded)
			{
				//如果使用重力
				velocity.y += 3.5 * g * deltaTime;

			}
		}

		//处理力和速度的关系
		//F = ma

		velocity = (velocity * (1-damping)) + (sumForce * (deltaTime * invMass));

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

					if (!isTrigger)
					{
						isHitWall = true;
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
					if (!isTrigger)
					{
						isHitWall = true;
						newPosition.x = (int)newPosition.x;
						velocity.x = 0;
					}
					
				}
			}

			isGrounded = false;
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

					if (!isTrigger)
					{
						isHitWall = true;
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

					if (!isTrigger) {
						isHitWall = true;
						newPosition.y = (int)newPosition.y;
						velocity.y = 0;
						isGrounded = true;
					}
					
				}
			}

			

		}

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

	Rigidbody* Rigidbody::GetCollisionRigidbody()
	{
		return collisionRigid;
	}

	void Rigidbody::SetCollisionRigidbody(Rigidbody* rigid)
	{
		collisionRigid = rigid;
	}

	int Rigidbody::GetIndexInMap() const
	{
		return this->indexInMap;
	}

	bool IsCollisionTile(int tileID) 
	{
		return ContainKey<int, bool>(collisionTiles, tileID);
	}

	bool CollisionWithinRigidbody(Rigidbody * rigid,const int x,const int y)
	{

		//indexInMap是一个动态的概念
		const int indexInMap = y * tileMap->w + x;

		for (int i=0;i< rigidbodys.size();i++)
		{
			if (rigidbodys[i]==rigid)
			{
				continue;
			}
			if (rigidbodys[i]->GetIndexInMap() == indexInMap)
			{
				return true;
			}
		}
		return false;
	}

	bool CollisionWithinRigidbody(Rigidbody* rigid, const int x1, const int y1, const int x2, const int y2)
	{
		//indexInMap是一个动态的概念
		const int indexInMap1 = y1 * tileMap->w + x1;
		const int indexInMap2 = y2 * tileMap->w + x2;

		bool resultValue = false;

		for (int i = 0; i < rigidbodys.size(); i++)
		{
			if (rigidbodys[i] == rigid)
			{
				continue;
			}
			const int indexOfRigid = rigidbodys[i]->GetIndexInMap();
			if (indexOfRigid == indexInMap1||indexOfRigid == indexInMap2)
			{
				//Debug("发生碰撞");
				//const float m1 = rigid->mass;
				//const float m2 = rigidbodys[i]->mass;
				//const Vector<float> v1 = rigid->velocity;
				//const Vector<float> v2 = rigidbodys[i]->velocity;
				//rigid->velocity = ((v1 * m1 + v2 * m2) / (m1 + m2)) * 2.0 - v1;
				//rigidbodys[i]->velocity = ((v1 * m1 + v2 * m2) / (m1 + m2)) * 2.0 - v2;
				//给予目标一个推力
				//Debug("发生碰撞:"+to_string(indexInMap1)+"|"+to_string(indexInMap2) + ":" + to_string(rigid->GetIndexInMap()));
				//rigidbodys[i]->AddForce((rigid->velocity * (0.01*-rigid->mass)), Rigidbody::Impulse);
				rigid->SetCollisionRigidbody(rigidbodys[i]);
				resultValue = true;
				if (rigid->isTrigger||rigidbodys[i]->isTrigger)
				{
					resultValue = false;
				}
				return resultValue;
				//return true;
			}
		}
		rigid->SetCollisionRigidbody(nullptr);
		resultValue = false;
		return resultValue;
		//return false;
	}

	void ApplyCollision(Rigidbody* rigid,int posx,int posy)
	{
		//indexInMap是一个动态的概念
		const int indexInMap = posy * tileMap->w + posx;

		for (int i = 0; i < rigidbodys.size(); i++)
		{
			if (rigidbodys[i] == rigid)
			{
				continue;
			}
			const int indexOfRigid = rigidbodys[i]->GetIndexInMap();
			if (indexOfRigid == indexInMap)
			{
				const float m1 = rigid->mass;
				const float m2 = rigidbodys[i]->mass;
				const Vector<float> v1 = rigid->velocity;
				const Vector<float> v2 = rigidbodys[i]->velocity;
				rigid->velocity = ((v1 * m1 + v2 * m2) / (m1 + m2)) * 2.0 - v1;
				rigidbodys[i]->velocity = ((v1 * m1 + v2 * m2) / (m1 + m2)) * 2.0 - v2;
				Debug("物体碰撞");
				//实现物体相撞后的处理
				// 
				//rigidbodys[i]->AddForce((rigid->velocity * rigid->mass), Rigidbody::Impulse);
				//rigid->AddForce((rigidbodys[i]->velocity * rigidbodys[i]->mass), Rigidbody::Impulse);
				//给予目标一个推力
				//Debug("发生碰撞:" + to_string(indexInMap1) + "|" + to_string(indexInMap2) + ":" + to_string(rigid->GetIndexInMap()));
				//rigidbodys[i]->AddForce((rigid->velocity * (0.01*-rigid->mass)), Rigidbody::Impulse);
				//return true;
				
			}
		}
	}

}

