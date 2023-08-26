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
		//������Ʒ
		auto it = std::find(rigidbodys.begin(), rigidbodys.end(), this);
		if (it != rigidbodys.end())
		{
			rigidbodys.erase(it);
		}

		Debug("rigidbody has been done");

	}

	
	//ʵ������Ч��
	void Rigidbody::RigidbodyUpdate(float deltaTime)
	{
		if (useGravity)
		{
			if (!isGrounded)
			{
				//���ʹ������
				velocity.y += 3.5 * g * deltaTime;

			}
			else {
				if (velocity.y > 0)
				{
					velocity.y = 0;
				}
			}
		}

		//���������ٶȵĹ�ϵ
		//F = ma

		velocity += sumForce * deltaTime * invMass;

		//���ٶȵ��������������λ�Ʊ仯
		(*colliderPos) += (velocity * deltaTime);
		//����λ�ý����������ײ���
		//�����⵽����ײ�ַ�����ֹͣ

		
		//ApplyCollision();
		

	}

	void Rigidbody::AddForce(Vector<float> & force, ForceType forceType)
	{
		//�������������
		switch (forceType)
		{
		case Rigidbody::ContinuousForce:
			//��Ӻ�����������
			sumForce += force;
			break;
		case Rigidbody::Impulse:
			//���һ�������������ϣ����������Ͼ���ʧ
			velocity += force * invMass;
			break;
		default:
			break;
		}

	}
	void Rigidbody::SetCollisionTileID(std::vector<uint32_t> & collisionTileIDs)
	{
		//����Collision Tiles
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
		//����
		pos.x = int(colliderPos->x);
		pos.y = int(colliderPos->y) + colliderSize.y;
		int hitByte = GetCollisionTile(pos);
		if (hitByte == -1)
		{
			return;
		}
		if (ContainKey<Uint32, bool>(collisionTiles, hitByte) && collisionTiles[hitByte])
		{
			//���巵��
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
			//���巵��
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
			//���巵��
			//(*colliderPos) = (*colliderPos) - (velocity * deltaTime);
		}

		//����
		pos.x = int(colliderPos->x);
		pos.y = int(colliderPos->y) - colliderSize.y;
		hitByte = GetCollisionTile(pos);
		if (hitByte == -1)
		{
			return;
		}
		if (ContainKey<Uint32, bool>(collisionTiles, hitByte) && collisionTiles[hitByte])
		{
			//���巵��
			//(*colliderPos) = (*colliderPos) - (velocity * deltaTime);
		}

		//����
		pos.x = int(colliderPos->x) - colliderSize.x;
		pos.y = int(colliderPos->y) - colliderSize.y;
		hitByte = GetCollisionTile(pos);
		if (hitByte == -1)
		{
			return;
		}
		if (ContainKey<Uint32, bool>(collisionTiles, hitByte) && collisionTiles[hitByte])
		{
			//���巵��
			//(*colliderPos) = (*colliderPos) - (velocity * deltaTime);
		}
		

		//����
		pos.x = int(colliderPos->x) + colliderSize.x;
		pos.y = int(colliderPos->y) - colliderSize.y;
		hitByte = GetCollisionTile(pos);
		if (hitByte == -1)
		{
			return;
		}
		if (ContainKey<Uint32, bool>(collisionTiles, hitByte) && collisionTiles[hitByte])
		{
			//���巵��
			//(*colliderPos) = (*colliderPos) - (velocity * deltaTime);
		}

	}

	float Rigidbody::FixPosition()
	{
		//�޸����λ��
		return 0.0;
	}

	int GetCollisionTile(Vector<float>& colliderPos)
	{
		//�����Һ͵�ͼ����ײ
		//�����⵽�ĵ�λ��byte�Ǻϣ��ͷ���true
		if (levelMap == nullptr)
		{
			return -1;
		}
		const int tileIndex = (int)(colliderPos.y) * levelMap->w + (int)colliderPos.x;
		const int hitByte = levelMap->level[tileIndex];
		return hitByte;
	}

}

