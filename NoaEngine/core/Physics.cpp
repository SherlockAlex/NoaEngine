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
	extern bool CollisionWithinRigidbody(Rigidbody* rigid,int x,int y);

	vector<Rigidbody*> rigidbodys;
	TileMap* tileMap = nullptr;

	Vector<float> sumForce(0.0, 0.0);
	float invMass = 1;

	Rigidbody::Rigidbody(Transform* colliderPos)
	{
		this->colliderPos = colliderPos;
		invMass = 1.0 / mass;
		
		rigidbodys.push_back(this);
	}

	Rigidbody::~Rigidbody()
	{
		//销毁物品
		auto it = std::find(rigidbodys.begin(), rigidbodys.end(), this);
		if (it != rigidbodys.end())
		{
			rigidbodys.erase(it);
		}

		Debug("rigidbody has been done");

	}

	
	void Rigidbody::Start()
	{
	}

	//实现物理效果
	void Rigidbody::Update()
	{
		indexInMap = (int)(colliderPos->position.x) + (int)(colliderPos->position.y) * tileMap->w;
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

		velocity += move(sumForce * (deltaTime * invMass));

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
					||CollisionWithinRigidbody(this,newPosition.x,colliderPos->position.y)
					||CollisionWithinRigidbody(this, newPosition.x, colliderPos->position.y+0.999)
				)
					
				{
					newPosition.x = (int)newPosition.x + 1;
					velocity.x = 0;
				}
			}
			else
			{
				if (ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.999, colliderPos->position.y + 0.0))
					||ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.999, colliderPos->position.y + 0.999))
					|| CollisionWithinRigidbody(this, newPosition.x+0.999, colliderPos->position.y)
					|| CollisionWithinRigidbody(this, newPosition.x+0.999, colliderPos->position.y + 0.999)
					)
				{
					newPosition.x = (int)newPosition.x;
					velocity.x = 0;
				}
			}

			isGrounded = false;
			if (velocity.y <= 0)
			{
				if (ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.0f, newPosition.y))
					||ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.999f, newPosition.y))
					|| CollisionWithinRigidbody(this, newPosition.x, newPosition.y)
					|| CollisionWithinRigidbody(this, newPosition.x+0.999, newPosition.y)
					)
				{
					newPosition.y = (int)newPosition.y + 1;
					velocity.y = 0;
				}
			}
			else
			{
				if (ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.0f, newPosition.y + 0.999))
					||ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.999f, newPosition.y + 0.999))
					|| CollisionWithinRigidbody(this, newPosition.x, newPosition.y+0.999)
					|| CollisionWithinRigidbody(this, newPosition.x + 0.999, newPosition.y+0.999)
					)
				{
					newPosition.y = (int)newPosition.y;
					velocity.y = 0;
					isGrounded = true;
				}
			}

			

		}

		colliderPos->position = move(newPosition);
		//Debug("isGrounded:" + to_string(isGrounded));

	}

	void Rigidbody::AddForce(Vector<float> & force, ForceType forceType)
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
	void Rigidbody::ApplyCollision()
	{
		

	}

	float Rigidbody::FixPosition()
	{
		//修复玩家位置
		return 0.0;
	}

	void Rigidbody::UpdateCollision(Vector<float>& nextPosition)
	{
		
	}

	int Rigidbody::GetIndexInMap()
	{
		return this->indexInMap;
	}

	bool IsCollisionTile(int tileID) {
		return ContainKey<int, bool>(collisionTiles, tileID);
	}

	bool CollisionWithinRigidbody(Rigidbody * rigid,int x, int y)
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

}

