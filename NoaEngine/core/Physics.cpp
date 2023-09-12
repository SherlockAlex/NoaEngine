#include "Physics.h"
#include "NoaEngine.h"
#include "Scene.h"
#include <unordered_map>
#include <thread>

using namespace std;

namespace noa {

	extern float deltaTime;
	//���rigid�Ƿ������������ײ�������ײ�ͷ���
	extern bool CollisionWithinRigidbody(Rigidbody* rigid, const int x1, const int y1,const int x2,const int y2);

	extern void ApplyRigidbodysCollision();

	unordered_map<size_t,Rigidbody*> rigidbodys;
	unordered_map<size_t, bool> isCheckCollision;

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
		this->newPosition = colliderPos->position;
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
		
		//�����ٶȽ����������ײ���
		//�����⵽����ײ�ַ�����ֹͣ
		if (collision.isTrigger && collision.other != nullptr)
		{
			OnTrigger(collision.other);
		}
		collision.other = nullptr;

		//indexInMap = (int)(colliderPos->position.x) + (int)(colliderPos->position.y) * tileMap->w;
		const int x = colliderPos->position.x;
		const int y = colliderPos->position.y;
		indexInMap = (x << 16) | y;

		collision.other = nullptr;
		collision.isHitCollisionTile = false;

		if (useMotion&&useGravity&&(!collision.isGrounded)) 
		{
			velocity.y += 3.5 * g * deltaTime;
		}

		if (useMotion)
		{

			//���������ٶȵĹ�ϵ
			//F = ma
			velocity = (velocity * (1 - damping)) + (sumForce * (deltaTime * invMass));
			newPosition = (colliderPos->position) + (velocity * deltaTime);
			ApplyCollision();
			//������ײ���
			colliderPos->position = newPosition;
		}
		

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

	void Rigidbody::UpdateMap(TileMap* map)
	{
		this->tileMap = map;
		if (this->tileMap==nullptr)
		{
			Debug("Load tile map failed");
			exit(-1);
		}
	}

	Vector<float> pos(0.0, 0.0);

	void Rigidbody::ApplyCollision()
	{
		if (!useCollision||tileMap == nullptr)
		{
			return;
		}

		if (velocity.x <= 0)
		{

			if (
				CollisionWithinRigidbody(this
					, newPosition.x, colliderPos->position.y
					, newPosition.x, colliderPos->position.y + 0.999
				)
				|| tileMap->IsCollisionTile(newPosition.x, colliderPos->position.y)
				|| tileMap->IsCollisionTile(newPosition.x, colliderPos->position.y + 0.999)
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
		else if (
				CollisionWithinRigidbody(this
					, newPosition.x + 0.999, colliderPos->position.y
					, newPosition.x + 0.999, colliderPos->position.y + 0.999
				)
				|| tileMap->IsCollisionTile(newPosition.x + 0.999, colliderPos->position.y)
				|| tileMap->IsCollisionTile(newPosition.x + 0.999, colliderPos->position.y + 0.999)
				)
		{
			if (!collision.isTrigger)
			{
				collision.isHitCollisionTile = true;
				newPosition.x = (int)newPosition.x;
				velocity.x = 0;
			}

		}

		collision.isGrounded = false;
		if (velocity.y <= 0)
		{
			if (
				CollisionWithinRigidbody(this,
					newPosition.x, newPosition.y
					, newPosition.x + 0.999, newPosition.y
				)
				|| tileMap->IsCollisionTile(newPosition.x, newPosition.y)
				|| tileMap->IsCollisionTile(newPosition.x + 0.999, newPosition.y)
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
		else if (
				CollisionWithinRigidbody(this
					, newPosition.x, newPosition.y + 0.999
					, newPosition.x + 0.999, newPosition.y + 0.999
				)
				|| tileMap->IsCollisionTile(newPosition.x, newPosition.y + 0.999)
				|| tileMap->IsCollisionTile(newPosition.x + 0.999f, newPosition.y + 0.999)
				)

		{

			if (!collision.isTrigger) {
				collision.isHitCollisionTile = true;
				newPosition.y = (int)newPosition.y;
				velocity.y = 0;
				collision.isGrounded = true;
			}

		}

		//if (velocity.x <= 0)
		//{
		//	//����
		//	if (
		//		CollisionWithinRigidbody(this
		//			, newPosition.x - colliderSize.x - 0.4999, colliderPos->position.y - colliderSize.y - 0.4999
		//			, newPosition.x - colliderSize.x - 0.4999, colliderPos->position.y + 0.4999 + colliderSize.y
		//		)
		//		|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x - colliderSize.x - 0.4999, colliderPos->position.y - colliderSize.y - 0.4999))
		//		|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x - colliderSize.x - 0.4999, colliderPos->position.y + colliderSize.y + 0.4999))
		//		)

		//	{

		//		if (!collision.isTrigger)
		//		{
		//			collision.isHitCollisionTile = true;
		//			//newPosition.x = (int)newPosition.x + 1 + colliderSize.x + 0.5;
		//			newPosition.x = (int)newPosition.x + colliderSize.x + 0.5;
		//			velocity.x = 0;
		//		}

		//	}
		//}
		//else
		//{
		//	if (
		//		CollisionWithinRigidbody(this
		//			, newPosition.x + 0.4999 + colliderSize.x, colliderPos->position.y - colliderSize.y - 0.4999
		//			, newPosition.x + 0.4999 + colliderSize.x, colliderPos->position.y + 0.4999 + colliderSize.y
		//		)
		//		|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.4999 + colliderSize.x, colliderPos->position.y - colliderSize.y- 0.4999))
		//		|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.4999 + colliderSize.x, colliderPos->position.y + 0.4999 + colliderSize.y))
		//		)
		//	{
		//		if (!collision.isTrigger)
		//		{
		//			collision.isHitCollisionTile = true;
		//			newPosition.x = (int)newPosition.x + 0.5 - colliderSize.x;
		//			velocity.x = 0;
		//		}

		//	}
		//}

		//collision.isGrounded = false;
		//if (velocity.y <= 0)
		//{
		//	if (
		//		CollisionWithinRigidbody(this,
		//			newPosition.x - colliderSize.x - 0.4999, newPosition.y - colliderSize.y - 0.4999
		//			, newPosition.x + 0.4999 + colliderSize.x, newPosition.y - colliderSize.y - 0.4999
		//		)
		//		|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x - colliderSize.x - 0.4999, newPosition.y - colliderSize.y - 0.4999))
		//		|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.4999 + colliderSize.x, newPosition.y - colliderSize.y - 0.4999))
		//		)
		//	{

		//		if (!collision.isTrigger)
		//		{
		//			collision.isHitCollisionTile = true;
		//			//newPosition.y = (int)newPosition.y + 1 + colliderSize.y + 0.5;
		//			newPosition.y = (int)newPosition.y + colliderSize.y + 0.5;
		//			velocity.y = 0;
		//		}
		//	}
		//}
		//else
		//{
		//	if (
		//		CollisionWithinRigidbody(this
		//			, newPosition.x - colliderSize.x - 0.4999, newPosition.y + 0.4999 + colliderSize.y
		//			, newPosition.x + 0.4999 + colliderSize.x, newPosition.y + 0.4999 + colliderSize.y
		//		)
		//		|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x - colliderSize.x - 0.4999, newPosition.y + 0.4999 + colliderSize.y))
		//		|| ContainKey<int, bool>(collisionTiles, tileMap->GetTileID(newPosition.x + 0.4999 + colliderSize.x, newPosition.y + 0.4999 + colliderSize.y))
		//		)

		//	{

		//		if (!collision.isTrigger) {
		//			collision.isHitCollisionTile = true;
		//			newPosition.y = (int)newPosition.y - colliderSize.y + 0.5;
		//			velocity.y = 0;
		//			collision.isGrounded = true;
		//		}

		//	}
		//}

	}

	int Rigidbody::GetIndexInMap() const
	{
		return this->indexInMap;
	}

	void Rigidbody::RemoveRigidbody() const
	{
		DestroyRigidbody(this);
	}

	


	bool CollisionWithinRigidbody(Rigidbody* rigid, const int x1, const int y1, const int x2, const int y2)
	{

		//��Ҫ���д��Ż����ظ���������
		//������������֮�䣬ֻҪ����һ����ײ�����
		//ͬʱ������֮����ײ�ĸ��嵽����Ϣ��
		//Ȼ��rigid��ȥ��λά����[x1][y1][x2][y2]���鿴�������֮��ײ�ĸ����Ƿ�Ϊ��
		//����֮�����ײֻҪÿһ֡ȥִ��һ�α��

		if (rigid == nullptr||rigid->isFrozen)
		{
			return false;
		}

		//indexInMap��һ����̬�ĸ����ȡΨһ��mapֵ
		const int indexInMap1 = (x1<<16)|y1;
		const int indexInMap2 = (x2<<16)|y2;

		const size_t hashCode = rigid->GetHashCode();

		for (const auto & pair:rigidbodys) 
		{
			if (pair.first == hashCode||pair.second == nullptr)
			{
				continue;
			}
			Rigidbody* rigidbody = pair.second;
			const int indexOfRigid = rigidbody->GetIndexInMap();
			if (indexOfRigid!=indexInMap1&&indexOfRigid!=indexInMap2)
			{
				continue;
			}

			rigid->collision.other = rigidbody;
			rigidbody->collision.other = rigid;
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

	void ApplyRigidbodysCollision() 
	{
		//���������ײ

		//������������[0][0]:��
		//������������[0][1]:��
		//������������[1][0]:��
		//������������[1][1]:��

		
		

	}

}

