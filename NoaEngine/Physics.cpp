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
		}

		//���������ٶȵĹ�ϵ
		//F = ma

		velocity += sumForce * deltaTime * invMass;

		//���ٶȵ��������������λ�Ʊ仯
		Vector<float> newPosition= move((*colliderPos) + (velocity * deltaTime));
		
		//�����ٶȽ����������ײ���
		//�����⵽����ײ�ַ�����ֹͣ

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
	void Rigidbody::SetCollisionTileID(std::vector<int> collisionTileIDs)
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
		tileMap = static_cast<TileMap*>(map);
	}

	Vector<float> pos(0.0, 0.0);
	void Rigidbody::ApplyCollision()
	{
		

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
		if (tileMap == nullptr)
		{
			return -1;
		}
		const int tileIndex = (int)(colliderPos.y) * tileMap->w + (int)colliderPos.x;
		const int hitByte = tileMap->level[tileIndex];
		return hitByte;
	}

}

