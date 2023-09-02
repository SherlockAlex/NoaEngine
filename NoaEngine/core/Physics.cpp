#include "Physics.h"
#include "NoaEngine.h"
#include "Scene.h"
#include <unordered_map>

using namespace std;

namespace noa {

	extern float deltaTime;

	unordered_map<int,bool> collisionTiles;

	extern bool IsCollisionTile(int tileID);
	//���rigid�Ƿ������������ײ�������ײ�ͷ���
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
		//������Ʒ
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

	//ʵ������Ч��
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
				//���ʹ������
				velocity.y += 3.5 * g * deltaTime;

			}
		}

		//���������ٶȵĹ�ϵ
		//F = ma

		velocity += move(sumForce * (deltaTime * invMass));

		//���ٶȵ��������������λ�Ʊ仯
		Vector<float> newPosition= move((colliderPos->position) + (velocity * deltaTime));
		
		//�����ٶȽ����������ײ���
		//�����⵽����ײ�ַ�����ֹͣ

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

		//indexInMap��һ����̬�ĸ���
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

