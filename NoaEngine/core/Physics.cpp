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

	//ʵ������Ч��
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
				//���ʹ������
				velocity.y += 3.5 * g * deltaTime;

			}
		}

		//���������ٶȵĹ�ϵ
		//F = ma

		velocity = (velocity * (1-damping)) + (sumForce * (deltaTime * invMass));

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

	float Rigidbody::FixPosition()
	{
		//�޸����λ��
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

	bool CollisionWithinRigidbody(Rigidbody* rigid, const int x1, const int y1, const int x2, const int y2)
	{
		//indexInMap��һ����̬�ĸ���
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
				//Debug("������ײ");
				//const float m1 = rigid->mass;
				//const float m2 = rigidbodys[i]->mass;
				//const Vector<float> v1 = rigid->velocity;
				//const Vector<float> v2 = rigidbodys[i]->velocity;
				//rigid->velocity = ((v1 * m1 + v2 * m2) / (m1 + m2)) * 2.0 - v1;
				//rigidbodys[i]->velocity = ((v1 * m1 + v2 * m2) / (m1 + m2)) * 2.0 - v2;
				//����Ŀ��һ������
				//Debug("������ײ:"+to_string(indexInMap1)+"|"+to_string(indexInMap2) + ":" + to_string(rigid->GetIndexInMap()));
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
		//indexInMap��һ����̬�ĸ���
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
				Debug("������ײ");
				//ʵ��������ײ��Ĵ���
				// 
				//rigidbodys[i]->AddForce((rigid->velocity * rigid->mass), Rigidbody::Impulse);
				//rigid->AddForce((rigidbodys[i]->velocity * rigidbodys[i]->mass), Rigidbody::Impulse);
				//����Ŀ��һ������
				//Debug("������ײ:" + to_string(indexInMap1) + "|" + to_string(indexInMap2) + ":" + to_string(rigid->GetIndexInMap()));
				//rigidbodys[i]->AddForce((rigid->velocity * (0.01*-rigid->mass)), Rigidbody::Impulse);
				//return true;
				
			}
		}
	}

}

