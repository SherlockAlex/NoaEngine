#include "Physics.h"
#include "NoaEngine.h"
#include "Scene.h"
#include <unordered_map>
#include <thread>

using namespace std;

namespace noa {

	extern float deltaTime;

	unordered_map<int,bool> collisionTiles;

	extern bool IsCollisionTile(int tileID);
	//���rigid�Ƿ������������ײ�������ײ�ͷ���
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

	// ��ʼ����̬������
	size_t Rigidbody::nextId = 0;

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
		if (isFrozen)
		{
			return;
		}
		//indexInMap = (int)(colliderPos->position.x) + (int)(colliderPos->position.y) * tileMap->w;
		const int x = colliderPos->position.x;
		const int y = colliderPos->position.y;
		indexInMap = (x << 16) | y;

		collision.other = nullptr;
		collision.isHitCollisionTile = false;

		if (useMotion)
		{
			if (useGravity)
			{
				if (!collision.isGrounded)
				{
					//���ʹ������
					velocity.y += 3.5 * g * deltaTime;

				}
			}

			//���������ٶȵĹ�ϵ
			//F = ma

			velocity = (velocity * (1 - damping)) + (sumForce * (deltaTime * invMass));

			//���ٶȵ��������������λ�Ʊ仯
			newPosition = move((colliderPos->position) + (velocity * deltaTime));

			ApplyCollision();

			colliderPos->position = move(newPosition);
		}
		//�����ٶȽ����������ײ���
		//�����⵽����ײ�ַ�����ֹͣ
		

		if (collision.isTrigger && collision.other != nullptr)
		{
			OnTrigger(collision.other);
		}
		collision.other = nullptr;
		

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

	void Rigidbody::ApplyCollision()
	{
		if (!useCollision)
		{
			return;
		}

		//if (velocity.x <= 0)
		//{



		//	if (
		//		CollisionWithinRigidbody(this
		//			, newPosition.x - colliderSize.x, colliderPos->position.y - colliderSize.y
		//			, newPosition.x - colliderSize.x, colliderPos->position.y + 0.999 + colliderSize.y
		//		)
		//		|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x - colliderSize.x, colliderPos->position.y - colliderSize.y))
		//		|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x - colliderSize.x, colliderPos->position.y + 0.999 + colliderSize.y))
		//		//||CollisionWithinRigidbody(this, newPosition.x, colliderPos->position.y+0.999)
		//		)

		//	{

		//		if (!collision.isTrigger)
		//		{
		//			collision.isHitCollisionTile = true;
		//			newPosition.x = (int)newPosition.x + 1 + colliderSize.x;
		//			velocity.x = 0;
		//		}

		//	}
		//}
		//else
		//{
		//	if (
		//		CollisionWithinRigidbody(this
		//			, newPosition.x + 0.999 + colliderSize.x, colliderPos->position.y - colliderSize.y
		//			, newPosition.x + 0.999 + colliderSize.x, colliderPos->position.y + 0.999 + colliderSize.y
		//		)
		//		|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.999 + colliderSize.x, colliderPos->position.y - colliderSize.y))
		//		|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.999 + colliderSize.x, colliderPos->position.y + 0.999 + colliderSize.y))
		//		//|| CollisionWithinRigidbody(this, newPosition.x+0.999, colliderPos->position.y + 0.999)
		//		)
		//	{
		//		if (!collision.isTrigger)
		//		{
		//			collision.isHitCollisionTile = true;
		//			newPosition.x = (int)newPosition.x - colliderSize.x;
		//			velocity.x = 0;
		//		}

		//	}
		//}

		//collision.isGrounded = false;
		//if (velocity.y <= 0)
		//{
		//	if (
		//		CollisionWithinRigidbody(this,
		//			newPosition.x - colliderSize.x, newPosition.y - colliderSize.y
		//			, newPosition.x + 0.999 + colliderSize.x, newPosition.y - colliderSize.y
		//		)
		//		|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x - colliderSize.x, newPosition.y - colliderSize.y))
		//		|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.999f + colliderSize.x, newPosition.y - colliderSize.y))
		//		//|| CollisionWithinRigidbody(this, newPosition.x+0.999, newPosition.y)
		//		)
		//	{

		//		if (!collision.isTrigger)
		//		{
		//			collision.isHitCollisionTile = true;
		//			newPosition.y = (int)newPosition.y + 1 + colliderSize.y;
		//			velocity.y = 0;
		//		}
		//	}
		//}
		//else
		//{
		//	if (
		//		CollisionWithinRigidbody(this
		//			, newPosition.x - colliderSize.x, newPosition.y + 0.999 + colliderSize.y
		//			, newPosition.x + 0.999 + colliderSize.x, newPosition.y + 0.999 + colliderSize.y
		//		)
		//		|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x - colliderSize.x, newPosition.y + 0.999 + colliderSize.y))
		//		|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.999f + colliderSize.x, newPosition.y + 0.999 + colliderSize.y))
		//		//|| CollisionWithinRigidbody(this, newPosition.x + 0.999, newPosition.y+0.999)
		//		)

		//	{

		//		if (!collision.isTrigger) {
		//			collision.isHitCollisionTile = true;
		//			newPosition.y = (int)newPosition.y - colliderSize.y;
		//			velocity.y = 0;
		//			collision.isGrounded = true;
		//		}

		//	}
		//}

		if (velocity.x <= 0)
		{
			//����
			if (
				CollisionWithinRigidbody(this
					, newPosition.x - colliderSize.x - 0.499, colliderPos->position.y - colliderSize.y -0.499
					, newPosition.x - colliderSize.x - 0.499, colliderPos->position.y + 0.499 + colliderSize.y
				)
				|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x - colliderSize.x - 0.499, colliderPos->position.y - colliderSize.y - 0.499))
				|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x - colliderSize.x - 0.499, colliderPos->position.y + colliderSize.y + 0.499))
				//||CollisionWithinRigidbody(this, newPosition.x, colliderPos->position.y+0.999)
				)

			{

				if (!collision.isTrigger)
				{
					collision.isHitCollisionTile = true;
					//newPosition.x = (int)newPosition.x + 1 + colliderSize.x + 0.5;
					newPosition.x = (int)newPosition.x + colliderSize.x + 0.5;
					velocity.x = 0;
				}

			}
		}
		else
		{
			if (
				CollisionWithinRigidbody(this
					, newPosition.x + 0.499 + colliderSize.x, colliderPos->position.y - colliderSize.y - 0.499
					, newPosition.x + 0.499 + colliderSize.x, colliderPos->position.y + 0.499 + colliderSize.y
				)
				|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.499 + colliderSize.x, colliderPos->position.y - colliderSize.y-0.499))
				|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.499 + colliderSize.x, colliderPos->position.y + 0.499 + colliderSize.y))
				//|| CollisionWithinRigidbody(this, newPosition.x+0.999, colliderPos->position.y + 0.999)
				)
			{
				if (!collision.isTrigger)
				{
					collision.isHitCollisionTile = true;
					newPosition.x = (int)newPosition.x + 0.5 - colliderSize.x;
					velocity.x = 0;
				}

			}
		}

		collision.isGrounded = false;
		if (velocity.y <= 0)
		{
			if (
				CollisionWithinRigidbody(this,
					newPosition.x - colliderSize.x - 0.499, newPosition.y - colliderSize.y - 0.499
					, newPosition.x + 0.499 + colliderSize.x, newPosition.y - colliderSize.y -0.499
				)
				|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x - colliderSize.x - 0.499, newPosition.y - colliderSize.y - 0.499))
				|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.499 + colliderSize.x, newPosition.y - colliderSize.y - 0.499))
				//|| CollisionWithinRigidbody(this, newPosition.x+0.999, newPosition.y)
				)
			{

				if (!collision.isTrigger)
				{
					collision.isHitCollisionTile = true;
					//newPosition.y = (int)newPosition.y + 1 + colliderSize.y + 0.5;
					newPosition.y = (int)newPosition.y + colliderSize.y + 0.5;
					velocity.y = 0;
				}
			}
		}
		else
		{
			if (
				CollisionWithinRigidbody(this
					, newPosition.x - colliderSize.x - 0.499, newPosition.y + 0.499 + colliderSize.y
					, newPosition.x + 0.499 + colliderSize.x, newPosition.y + 0.499 + colliderSize.y
				)
				|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x - colliderSize.x -0.499, newPosition.y + 0.499 + colliderSize.y))
				|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.499 + colliderSize.x, newPosition.y + 0.499 + colliderSize.y))
				//|| CollisionWithinRigidbody(this, newPosition.x + 0.999, newPosition.y+0.999)
				)

			{

				if (!collision.isTrigger) {
					collision.isHitCollisionTile = true;
					newPosition.y = (int)newPosition.y - colliderSize.y + 0.5;
					velocity.y = 0;
					collision.isGrounded = true;
				}

			}
		}

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


	bool CollisionWithinRigidbody(Rigidbody* rigid, const int x1, const int y1, const int x2, const int y2)
	{

		if (rigid == nullptr||rigid->isFrozen)
		{
			return false;
		}

		//indexInMap��һ����̬�ĸ����ȡΨһ��mapֵ
		const int indexInMap1 = (x1<<16)|y1;
		const int indexInMap2 = (x2<<16)|y2;

		bool resultValue = false;

		const size_t hashCode = rigid->GetHashCode();

		for (auto & pair:rigidbodys) 
		{
			if (pair.first == hashCode||pair.second == nullptr)
			{
				continue;
			}
			Rigidbody* rigidbody = pair.second;
			const int indexOfRigid = rigidbody->GetIndexInMap();
			if (indexOfRigid^indexInMap1&&indexOfRigid^indexInMap2)
			{
				continue;
			}

			rigid->collision.other = rigidbody;
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
}

