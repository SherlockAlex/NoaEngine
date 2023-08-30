#include "Physics.h"
#include "NoaEngine.h"
#include "Scene.h"
#include <unordered_map>

using namespace std;

namespace noa {
	
	int GetCollisionTile(Vector<float>& colliderPos);

	unordered_map<int,bool> collisionTiles;

	vector<Rigidbody*> rigidbodys;
	TileMap* tileMap = nullptr;

	Vector<float> sumForce(0.0, 0.0);
	float invMass = 1;

	Rigidbody::Rigidbody(Vector<float>* colliderPos)
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

	
	//实现物理效果
	void Rigidbody::RigidbodyUpdate(float deltaTime)
	{
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

		velocity += sumForce * deltaTime * invMass;

		//将速度的量反馈到物体的位移变化
		Vector<float> newPosition= move((*colliderPos) + (velocity * deltaTime));
		
		//根据速度进行物体的碰撞检测
		//如果检测到了碰撞字符，就停止

		if (useCollision)
		{
			if (velocity.x <= 0)
			{
				if (ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x, colliderPos->y + 0.0)) ||
					ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x, colliderPos->y + 0.999)))
				{
					newPosition.x = (int)newPosition.x + 1;
					velocity.x = 0;
				}
			}
			else
			{
				if (ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.999, colliderPos->y + 0.0)) ||
					ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.999, colliderPos->y + 0.999)))
				{
					newPosition.x = (int)newPosition.x;
					velocity.x = 0;
				}
			}

			isGrounded = false;
			if (velocity.y <= 0)
			{
				if (ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.0f, newPosition.y)) ||
					ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.999f, newPosition.y)))
				{
					newPosition.y = (int)newPosition.y + 1;
					velocity.y = 0;
				}
			}
			else
			{
				if (ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.0f, newPosition.y + 0.999)) ||
					ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.999f, newPosition.y + 0.999)))
				{
					newPosition.y = (int)newPosition.y;
					velocity.y = 0;
					isGrounded = true;
				}
			}
		}

		*colliderPos = newPosition;
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

	int GetCollisionTile(Vector<float>& colliderPos)
	{
		//检测玩家和地图的碰撞
		//如果检测到的点位和byte吻合，就返回true
		if (tileMap == nullptr)
		{
			return -1;
		}
		const int tileIndex = (int)(colliderPos.y) * tileMap->w + (int)colliderPos.x;
		const int hitByte = tileMap->level[tileIndex];
		return hitByte;
	}

}

