#include "Physics.h"
#include "NoaEngine.h"
#include "Scene.h"
#include <unordered_map>

using namespace std;

namespace noa {
	
	int GetCollisionTile(Vector<float>& colliderPos);

	unordered_map<Uint32,bool> collisionTiles;

	vector<Rigidbody*> rigidbodys;
	LevelMap* levelMap = nullptr;

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
			else {
				if (velocity.y > 0)
				{
					velocity.y = 0;
				}
			}
		}

		//处理力和速度的关系
		//F = ma

		velocity += sumForce * deltaTime * invMass;

		//将速度的量反馈到物体的位移变化
		(*colliderPos) += (velocity * deltaTime);
		//根据位置进行物体的碰撞检测
		//如果检测到了碰撞字符，就停止

		
		//ApplyCollision();
		

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
	void Rigidbody::SetCollisionTileID(std::vector<uint32_t> & collisionTileIDs)
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
		levelMap = static_cast<LevelMap*>(map);
	}

	Vector<float> pos(0.0, 0.0);
	void Rigidbody::ApplyCollision()
	{
		//向下
		pos.x = int(colliderPos->x);
		pos.y = int(colliderPos->y) + colliderSize.y;
		int hitByte = GetCollisionTile(pos);
		if (hitByte == -1)
		{
			return;
		}
		if (ContainKey<Uint32, bool>(collisionTiles, hitByte) && collisionTiles[hitByte])
		{
			//刚体返回
			//(*colliderPos) = (*colliderPos) - (velocity * deltaTime);
		}

		pos.x = int(colliderPos->x) - colliderSize.x;
		pos.y = int(colliderPos->y) + colliderSize.y;
		hitByte = GetCollisionTile(pos);
		if (hitByte == -1)
		{
			return;
		}
		if (ContainKey<Uint32, bool>(collisionTiles, hitByte) && collisionTiles[hitByte])
		{
			//刚体返回
			//(*colliderPos) = (*colliderPos) - (velocity * deltaTime);
		}

		pos.x = int(colliderPos->x) + colliderSize.x;
		pos.y = int(colliderPos->y) + colliderSize.y;
		hitByte = GetCollisionTile(pos);
		if (hitByte == -1)
		{
			return;
		}
		if (ContainKey<Uint32, bool>(collisionTiles, hitByte) && collisionTiles[hitByte])
		{
			//刚体返回
			//(*colliderPos) = (*colliderPos) - (velocity * deltaTime);
		}

		//向上
		pos.x = int(colliderPos->x);
		pos.y = int(colliderPos->y) - colliderSize.y;
		hitByte = GetCollisionTile(pos);
		if (hitByte == -1)
		{
			return;
		}
		if (ContainKey<Uint32, bool>(collisionTiles, hitByte) && collisionTiles[hitByte])
		{
			//刚体返回
			//(*colliderPos) = (*colliderPos) - (velocity * deltaTime);
		}

		//向上
		pos.x = int(colliderPos->x) - colliderSize.x;
		pos.y = int(colliderPos->y) - colliderSize.y;
		hitByte = GetCollisionTile(pos);
		if (hitByte == -1)
		{
			return;
		}
		if (ContainKey<Uint32, bool>(collisionTiles, hitByte) && collisionTiles[hitByte])
		{
			//刚体返回
			//(*colliderPos) = (*colliderPos) - (velocity * deltaTime);
		}
		

		//向上
		pos.x = int(colliderPos->x) + colliderSize.x;
		pos.y = int(colliderPos->y) - colliderSize.y;
		hitByte = GetCollisionTile(pos);
		if (hitByte == -1)
		{
			return;
		}
		if (ContainKey<Uint32, bool>(collisionTiles, hitByte) && collisionTiles[hitByte])
		{
			//刚体返回
			//(*colliderPos) = (*colliderPos) - (velocity * deltaTime);
		}

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
		if (levelMap == nullptr)
		{
			return -1;
		}
		const int tileIndex = (int)(colliderPos.y) * levelMap->w + (int)colliderPos.x;
		const int hitByte = levelMap->level[tileIndex];
		return hitByte;
	}

}

